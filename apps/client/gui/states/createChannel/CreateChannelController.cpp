#include "CreateChannelController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
CreateChannelController::CreateChannelController(std::shared_ptr<api::Session> sessionInit,
                                                 const StateFactory& stateFactoryInit,
                                                 std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void CreateChannelController::activate()
{
    session->addMessageHandler({common::messages::MessageId::CreateChannelResponse, createChannelResponseHandlerName,
                                [this](const auto& msg) { handleCreateChannelResponse(msg); }});
}

void CreateChannelController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::CreateChannelResponse, createChannelResponseHandlerName});
}

void CreateChannelController::goBack()
{
    LOG_S(INFO) << "Return to previous state";
    stateMachine->returnToThePreviousState();
}

void CreateChannelController::createChannel(const QString& channelName)
{
    LOG_S(INFO) << "Create channel with name: " << channelName.toStdString();

    nlohmann::json data{
        {"channelName", channelName.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::CreateChannel, data);
}

void CreateChannelController::handleCreateChannelResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    LOG_S(INFO) << "Handle create channel response";

    if (responseJson.contains("error"))
    {
        auto errorMessage =
            std::format("Error while creating channel: {}", responseJson.at("error").get<std::string>());

        emit createChannelFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.is_array() and responseJson.at(0).get<std::string>() == "ok")
    {
        stateMachine->returnToThePreviousState();

        LOG_S(INFO) << "Successfully created channel";
    }
}
}