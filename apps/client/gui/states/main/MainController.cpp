#include "MainController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
MainController::MainController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                               std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void MainController::activate()
{
    session->addMessageHandler({common::messages::MessageId::GetUserChannelsResponse,
                                getUserChannelsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserChannelsResponse(msg); }});

    session->sendMessage(common::messages::MessageId::GetUserChannels, {});
}

void MainController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::GetUserChannelsResponse, getUserChannelsResponseHandlerName});
}

void MainController::logout()
{
    LOG_S(INFO) << "Handle logout";

    stateMachine->returnToThePreviousState();
}

void MainController::goToCreateChannel()
{
    LOG_S(INFO) << "Handle go to create channel";

    stateMachine->addNextState(stateFactory.createCreateChannelState());
}

void MainController::handleGetUserChannelsResponse(const common::messages::Message& message) {}

void MainController::goToSendFriendRequest()
{
    LOG_S(INFO) << "Handle go to send friend request";

    stateMachine->addNextState(stateFactory.createSendFriendRequestState());
}

void MainController::goToUserSettings()
{
    LOG_S(INFO) << "Handle go to user settings";

    stateMachine->addNextState(stateFactory.createUserSettingsState());
}
}