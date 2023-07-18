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

void LoginController::loginRequest(const QString& email, const QString& password)
{
    nlohmann::json payload{
        {"email", email.toStdString()},
        {"password", password.toStdString()},
    };

    common::messages::Message message{common::messages::MessageId::Login, common::bytes::Bytes{payload.dump()}};

    session->sendMessage(message);

    LOG_S(INFO) << std::format("Sent login request for user {}", static_cast<std::string>(message.payload));

    nextState = stateFactory.createMainState();
}

void LoginController::goToRegisterState()
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

    nextState = std::nullopt;
}

void LoginController::handleLoginResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    LOG_S(INFO) << "Handle login response";

    if (responseJson.contains("error"))
    {
        auto errorMessage = std::format("Error while logging: {}", responseJson.at("error").get<std::string>());

        emit loginFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.contains("token"))
    {
        LOG_S(INFO) << "Successfully logged";

        session->storeToken(responseJson.at("token").get<std::string>());

        stateMachine->addNextState(nextState.value());
    }
}
}