#include "MainController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
MainController::MainController(std::shared_ptr<api::Session> sessionInit) : session{sessionInit} {}

void MainController::handleRegisterRequest(const QString& email, const QString& password)
{

    nlohmann::json payload{
        {"email", email.toStdString()},
        {"password", password.toStdString()},
    };

    common::messages::Message message{common::messages::MessageId::Register, common::messages::nullToken,
                                      common::bytes::Bytes{payload.dump()}};

    session->sendMessage(message);

    LOG_S(INFO) << std::format("Sent register request for user {}", static_cast<std::string>(message.payload));
}

}