#include "SendFriendRequestController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
SendFriendRequestController::SendFriendRequestController(std::shared_ptr<api::Session> sessionInit,
                                                         const StateFactory& stateFactoryInit,
                                                         std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void SendFriendRequestController::activate()
{
    session->addMessageHandler({common::messages::MessageId::CreateChannelResponse,
                                sendFriendRequestResponseHandlerName,
                                [this](const auto& msg) { handleSendFriendRequestResponse(msg); }});
}

void SendFriendRequestController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::CreateChannelResponse, sendFriendRequestResponseHandlerName});
}

void SendFriendRequestController::sendFriendRequest(const QString& friendEmail) {}

void SendFriendRequestController::goBack()
{
    stateMachine->returnToThePreviousState();
}

void SendFriendRequestController::handleSendFriendRequestResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    LOG_S(INFO) << "Handle send friend request response";

    if (responseJson.contains("error"))
    {
        auto errorMessage =
            std::format("Error while sending friend request: {}", responseJson.at("error").get<std::string>());

        emit sendFriendRequestFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.is_array() and responseJson.at(0).get<std::string>() == "ok")
    {
        LOG_S(INFO) << "Successfully sended friend request";

        stateMachine->returnToThePreviousState();
    }
}
}