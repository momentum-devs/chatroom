#pragma once

#include "client/api/Session.h"
#include "client/gui/qml/LoaderController.h"
#include "client/storage/ConversationStorage.h"
#include "State.h"
#include "StateMachine.h"

namespace client::gui
{
class StateFactory
{
public:
    StateFactory(std::shared_ptr<client::api::Session> session, std::shared_ptr<StateMachine> stateMachine,
                 std::shared_ptr<LoaderController> view);
    std::shared_ptr<State> createDefaultState() const;
    std::shared_ptr<State> createRegisterState() const;
    std::shared_ptr<State> createLoginState() const;
    std::shared_ptr<State> createChannelState(const std::string& channelId, const std::string& channelName,
                                              bool isOwner) const;
    std::shared_ptr<State> createCreateChannelState() const;
    std::shared_ptr<State> createSendFriendInvitationState() const;
    std::shared_ptr<State> createUserSettingsState() const;
    std::shared_ptr<State> createVerifyUserState() const;
    std::shared_ptr<State> createPrivateMessagesState() const;
    std::shared_ptr<State> createInviteToChannelState(const std::string& channelId) const;

private:
    std::shared_ptr<client::api::Session> session;
    std::shared_ptr<StateMachine> stateMachine;
    std::shared_ptr<LoaderController> loaderController;
    std::shared_ptr<storage::ConversationStorage> privateMessagesConversationStorage;
    std::shared_ptr<storage::ConversationStorage> channelConversationStorage;
};
}