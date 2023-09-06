#include "VerifyUserController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
VerifyUserController::VerifyUserController(std::shared_ptr<api::Session> sessionInit,
                                           const StateFactory& stateFactoryInit,
                                           std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void VerifyUserController::activate()
{
    session->addMessageHandler({common::messages::MessageId::VerifyUserResponse, verificationResponseHandlerName,
                                [this](const auto& msg) { handleVerificationResponse(msg); }});
}

void VerifyUserController::deactivate()
{
    session->removeMessageHandler({common::messages::MessageId::VerifyUserResponse, verificationResponseHandlerName});
}

void VerifyUserController::verificationRequest(const QString& verificationCode)
{
    nlohmann::json data{
        {"verificationCode", verificationCode.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::VerifyUser, data);

    LOG_S(INFO) << std::format("Sent verification request with code {}", verificationCode.toStdString());
}

void VerifyUserController::goToLoginState()
{
    stateMachine->clear(stateFactory.createLoginState());
}

void VerifyUserController::handleVerificationResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    LOG_S(INFO) << "Handle login response";

    if (responseJson.contains("error"))
    {
        auto errorMessage = std::format("Error while verifying user: {}", responseJson.at("error").get<std::string>());

        emit verificationFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }
    else
    {
        LOG_S(INFO) << "Successfully verified email";

        stateMachine->returnToThePreviousState();
    }
}
}