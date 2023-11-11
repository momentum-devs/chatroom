#pragma once

#include "client/api/Session.h"
#include "client/gui/qml/LoaderController.h"
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
    std::shared_ptr<State> createMainState() const;
    std::shared_ptr<State> createChannelState(const std::string& channelId, const std::string& channelName,
                                              bool isOwner) const;
    std::shared_ptr<State> createCreateChannelState() const;
    std::shared_ptr<State> createSendFriendRequestState() const;
    std::shared_ptr<State> createUserSettingsState() const;
    std::shared_ptr<State> createVerifyUserState() const;
    std::shared_ptr<State> createPrivateMessagesState() const;
    std::shared_ptr<State> createInviteToChannelState(const std::string& channelId) const;
    std::shared_ptr<State> createChannelMembersListState(const std::string& channelId) const;

private:
    std::shared_ptr<client::api::Session> session;
    std::shared_ptr<StateMachine> stateMachine;
    std::shared_ptr<LoaderController> loaderController;
};
}