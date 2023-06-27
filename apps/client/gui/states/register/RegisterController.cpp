#include "RegisterController.h"

#include <glog/logging.h>
#include <nlohmann/json.hpp>

namespace client::gui
{
RegisterController::RegisterController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                       std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void RegisterController::handleRegisterRequest(const QString& email, const QString& password)
{
    nlohmann::json payload{
        {"email", email.toStdString()},
        {"password", password.toStdString()},
    };

    common::messages::Message message{common::messages::MessageId::Register, common::bytes::Bytes{payload.dump()}};

    session->sendMessage(message);

    VLOG(0) << std::format("Sent register request for user {}", static_cast<std::string>(message.payload));
}

void RegisterController::handleGoBack()
{
    stateMachine->returnToThePreviousState();
}

void RegisterController::activate()
{
    session->addMessageHandler({common::messages::MessageId::RegisterResponse, registerResponseHandlerName,
                                [this](const auto& msg) { handleRegisterResponse(msg); }});
}

void RegisterController::deactivate()
{
    session->removeMessageHandler({common::messages::MessageId::LoginResponse, registerResponseHandlerName});
}

void RegisterController::handleRegisterResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    VLOG(0) << "Handle register response";

    if (responseJson.contains("error"))
    {
        auto errorMessage = std::format("Error while register user: {}", responseJson.at("error").get<std::string>());

        emit registerFailure(QString::fromStdString(errorMessage));

        VLOG(2) << errorMessage;
    }

    if (responseJson.contains("ok"))
    {
        stateMachine->returnToThePreviousState();

        VLOG(0) << "Successfully register";
    }
}
}