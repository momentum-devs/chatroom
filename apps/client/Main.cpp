#include <boost/asio.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QTranslator>

#include "api/SessionImpl.h"
#include "api/SocketConnectorImpl.h"
#include "common/environment/EnvironmentParser.h"
#include "common/filesystem/GetProjectPath.h"
#include "gui/controllers/MainController.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "messages/MessageReaderImpl.h"
#include "messages/MessageSenderImpl.h"
#include "messages/MessageSerializerImpl.h"

int main(int argc, char* argv[])
{
    auto dotEnvPath = common::filesystem::getProjectPath("chatroom") + "apps/client/.env";

    dotenv::init(dotEnvPath.c_str());

    common::environment::EnvironmentParser environmentParser;

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    auto serverHostname = environmentParser.parseString("SERVER_HOSTNAME");

    auto serverPort = static_cast<unsigned short>(std::stoi(environmentParser.parseString("SERVER_PORT")));

    auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    boost::asio::io_context context;

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);

    auto socketConnector = std::make_unique<client::api::SocketConnectorImpl>(socket);

    auto session = std::make_shared<client::api::SessionImpl>(std::move(messageReader), std::move(messageSender),
                                                              std::move(socketConnector));

    session->connect({serverHostname, serverPort});

    QGuiApplication app(argc, argv);

    QQuickView view;

    client::gui::MainController mainController{session};

    QObject::connect(&mainController, &client::gui::MainController::registerRequest, &mainController,
                     &client::gui::MainController::handleRegisterRequest);

    view.engine()->rootContext()->setContextProperty("mainController", &mainController);

    view.setSource(QUrl::fromLocalFile("chatroom/gui/views/MainView.qml"));

    view.show();

    std::thread api{[&] { context.run(); }};

    return app.exec();
}
