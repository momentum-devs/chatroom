#include "LoginController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
LoginController::LoginController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                 std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void LoginController::handleLoginRequest(const QString& email, const QString& password)
{
    nlohmann::json payload{
        {"email", email.toStdString()},
        {"password", password.toStdString()},
    };

    common::messages::Message message{common::messages::MessageId::Login, common::bytes::Bytes{payload.dump()}};

    session->sendMessage(message);

    LOG_S(INFO) << std::format("Sent login request for user {}", static_cast<std::string>(message.payload));
}

void LoginController::handleGoToRegisterState()
{
    stateMachine->addNextState(stateFactory.createRegisterState());
}
}