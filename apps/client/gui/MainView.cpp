#include "MainView.h"

#include "loguru.hpp"

namespace client::gui
{
MainView::MainView(std::shared_ptr<api::Session> sessionInit) :
    session{sessionInit}
{}

void MainView::handleRegisterRequest(const QString& username, const QString& password)
{
    common::messages::Message message{
        common::messages::MessageId::Register,
        common::messages::nullToken,
        common::bytes::Bytes{
            std::format("{{username: {}, password: {}}}", username.toStdString(), password.toStdString())
        }
    };

    session->sendMessage(message);

    LOG_S(INFO) << std::format("Sent register request for user {}", static_cast<std::string>(message.payload));
}

}