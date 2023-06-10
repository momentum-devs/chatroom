#include <boost/asio.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QTranslator>

#include "api/SessionImpl.h"
#include "api/SocketConnectorImpl.h"
#include "common/filesystem/GetProjectPath.h"
#include "config/ConfigProvider.h"
#include "gui/states/register/RegisterController.h"
#include "gui/states/StateFactory.h"
#include "gui/states/StateMachine.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "messages/MessageReaderImpl.h"
#include "messages/MessageSenderImpl.h"
#include "messages/MessageSerializerImpl.h"

int main(int argc, char* argv[])
{
    auto dotEnvPath = common::filesystem::getProjectPath("chatroom") + "/apps/client/.env";

    dotenv::init(dotEnvPath.c_str());

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    client::config::ConfigProvider configProvider;

    const auto serverHost = configProvider.getServerHost();

    const auto serverPort = configProvider.getServerPort();

    const auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    boost::asio::io_context context;

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);

    auto socketConnector = std::make_unique<client::api::SocketConnectorImpl>(socket);

    auto session = std::make_shared<client::api::SessionImpl>(std::move(messageReader), std::move(messageSender),
                                                              std::move(socketConnector));

    session->connect({serverHost, static_cast<unsigned short>(serverPort)});

    QGuiApplication app(argc, argv);

    auto view = std::make_shared<QQuickView>();

    auto stateMachine = std::make_shared<client::gui::StateMachine>();

    auto stateFactory = std::make_shared<client::gui::StateFactory>(session, stateMachine, view);

    stateMachine->addNextState(stateFactory->createDefaultState());

    std::thread api{[&] { context.run(); }};

    return app.exec();
}
