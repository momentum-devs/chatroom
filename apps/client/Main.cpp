#include <boost/asio.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QTranslator>

#include "api/SessionImpl.h"
#include "common/filesystem/GetProjectPath.h"
#include "config/ConfigProvider.h"
#include "gui/controllers/MainController.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "messages/MessageSerializerImpl.h"

int main(int argc, char* argv[])
{
    auto dotEnvPath = common::filesystem::getProjectPath("chatroom") + "apps/client/.env";

    dotenv::init(dotEnvPath.c_str());

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    client::config::ConfigProvider configProvider;

    const auto serverHost = configProvider.getServerHost();

    const auto serverPort = configProvider.getServerPort();

    const auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    boost::asio::io_context context;

    const auto session = std::make_shared<client::api::SessionImpl>(context, messageSerializer);

    session->connect(serverHost, serverPort);

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
