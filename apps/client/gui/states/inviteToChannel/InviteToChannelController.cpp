#include "InviteToChannelController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
InviteToChannelController::InviteToChannelController(std::shared_ptr<api::Session> sessionInit,
                                                     const StateFactory& stateFactoryInit,
                                                     std::shared_ptr<StateMachine> stateMachineInit,
                                                     const std::string& channelIdInit)
    : session{std::move(sessionInit)},
      stateFactory{stateFactoryInit},
      stateMachine{std::move(stateMachineInit)},
      channelId{channelIdInit}
{
}

void InviteToChannelController::activate()
{
    session->addMessageHandler({common::messages::MessageId::SendChannelInvitationResponse,
                                sendChannelInvitationResponseHandlerName,
                                [this](const auto& msg) { handleSendChannelInvitationResponse(msg); }});
}

void InviteToChannelController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::SendChannelInvitationResponse, sendChannelInvitationResponseHandlerName});
}

void InviteToChannelController::goBack()
{
    LOG_S(INFO) << "Return to previous state";

    stateMachine->returnToThePreviousState();
}

void InviteToChannelController::sendChannelInvitation(const QString& email)
{
    LOG_S(INFO) << fmt::format("Sending request to user with email: {} to chat with id: {}", email.toStdString(),
                               channelId);

    nlohmann::json data{
        {"channelId", channelId},
        {"email", email.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::SendChannelInvitation, data);
}

void InviteToChannelController::handleSendChannelInvitationResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << fmt::format("Handle send channel invitation response");

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        auto errorMessage =
            fmt::format("Error while sending invitation to channel: {}", responseJson.at("error").get<std::string>());

        emit sendChannelInvitationFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.is_array() and responseJson.at(0).get<std::string>() == "ok")
    {
        LOG_S(INFO) << "Successfully sent invitation to channel";

        stateMachine->returnToThePreviousState();
    }
}
}
