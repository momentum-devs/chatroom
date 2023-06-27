#include "LoginController.h"

#include <glog/logging.h>
#include <nlohmann/json.hpp>

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

    VLOG(0) << std::format("Sent login request for user {}", static_cast<std::string>(message.payload));
}

void LoginController::handleGoToRegisterState()
{
    stateMachine->addNextState(stateFactory.createRegisterState());
}

void LoginController::activate()
{
    session->addMessageHandler({common::messages::MessageId::LoginResponse, loginResponseHandlerName,
                                [this](const auto& msg) { handleLoginResponse(msg); }});
}

void LoginController::deactivate()
{
    session->removeMessageHandler({common::messages::MessageId::LoginResponse, loginResponseHandlerName});
}

void LoginController::handleLoginResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    VLOG(0) << "Handle login response";

    if (responseJson.contains("error"))
    {
        auto errorMessage = std::format("Error while logging: {}", responseJson.at("error").get<std::string>());

        emit loginFailure(QString::fromStdString(errorMessage));

        VLOG(2) << errorMessage;
    }

    if (responseJson.contains("token"))
    {
        // TODO: handle success

        VLOG(0) << "Successfully logged";
    }
}
}