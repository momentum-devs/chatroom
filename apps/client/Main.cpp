#include <boost/asio.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QTranslator>

#include "api/SessionImpl.h"
#include "common/environment/EnvironmentParser.h"
#include "common/filesystem/GetProjectPath.h"
#include "gui/MainView.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
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

    auto session = std::make_shared<client::api::SessionImpl>(context, messageSerializer);

    session->connect(serverHostname, serverPort);

    QGuiApplication app(argc, argv);

    QQuickView view;

    client::gui::MainView mainView{session};

    QObject::connect(&mainView, &client::gui::MainView::registerRequest,
                     &mainView, &client::gui::MainView::handleRegisterRequest);

    view.engine()->rootContext()->setContextProperty("MainView", &mainView);

    view.setSource(QUrl::fromLocalFile("chatroom/gui/qml/main.qml"));

    view.show();

    context.run();

    return app.exec();
}
