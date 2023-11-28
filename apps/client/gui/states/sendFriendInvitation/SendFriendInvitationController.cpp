#include <nlohmann/json.hpp>

#include "loguru.hpp"
#include "SendFriendInvitationController.h"

namespace client::gui
{
SendFriendInvitationController::SendFriendInvitationController(std::shared_ptr<api::Session> sessionInit,
                                                               const StateFactory& stateFactoryInit,
                                                               std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void SendFriendInvitationController::activate()
{
    session->addMessageHandler({common::messages::MessageId::SendFriendInvitationResponse,
                                sendFriendInvitationResponseHandlerName,
                                [this](const auto& msg) { handleSendFriendInvitationResponse(msg); }});
}

void SendFriendInvitationController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::SendFriendInvitationResponse, sendFriendInvitationResponseHandlerName});
}

void SendFriendInvitationController::sendFriendInvitation(const QString& friendEmail)
{
    LOG_S(INFO) << "Send invitation to friends to user " << friendEmail.toStdString();

    nlohmann::json payload{
        {"friend_email", friendEmail.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::SendFriendInvitation, payload);
}

void SendFriendInvitationController::goBack()
{
    LOG_S(INFO) << "Return to previous state";

    stateMachine->returnToThePreviousState();
}

void SendFriendInvitationController::handleSendFriendInvitationResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    LOG_S(INFO) << "Handle send friend request response";

    if (responseJson.contains("error"))
    {
        auto errorMessage =
            std::format("Error while sending friend request: {}", responseJson.at("error").get<std::string>());

        emit sendFriendInvitationFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.is_array() and responseJson.at(0).get<std::string>() == "ok")
    {
        LOG_S(INFO) << "Successfully sent friend request";

        stateMachine->returnToThePreviousState();
    }
}
}