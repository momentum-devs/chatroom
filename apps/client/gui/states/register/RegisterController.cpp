#include "RegisterController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
RegisterController::RegisterController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                       std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void RegisterController::registerRequest(const QString& email, const QString& nickname, const QString& password)
{
    nlohmann::json payload{
        {"email", email.toStdString()},
        {"nickname", nickname.toStdString()},
        {"password", password.toStdString()},
    };

    common::messages::Message message{common::messages::MessageId::Register, common::bytes::Bytes{payload.dump()}};

    session->sendMessage(message);

    LOG_S(INFO) << std::format("Sent register request for user {}", nickname.toStdString());
}

void RegisterController::goBack()
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
    session->removeMessageHandler({common::messages::MessageId::RegisterResponse, registerResponseHandlerName});
}

void RegisterController::handleRegisterResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    LOG_S(INFO) << "Handle register response";

    if (responseJson.contains("error"))
    {
        auto errorMessage = std::format("Error while register user: {}", responseJson.at("error").get<std::string>());

        emit registerFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.is_array() and responseJson.at(0).get<std::string>() == "ok")
    {
        stateMachine->returnToThePreviousState();

        LOG_S(INFO) << "Successfully register";
    }
}
}