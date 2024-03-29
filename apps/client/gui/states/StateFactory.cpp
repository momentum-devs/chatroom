#include "StateFactory.h"

#include "client/gui/controllers/LeftColumnController.h"
#include "client/gui/states/channel/ChannelState.h"
#include "client/storage/MessageStorage.h"
#include "createChannel/CreateChannelController.h"
#include "createChannel/CreateChannelState.h"
#include "inviteToChannel/InviteToChannelController.h"
#include "inviteToChannel/InviteToChannelState.h"
#include "login/LoginController.h"
#include "login/LoginState.h"
#include "privateMessages/PrivateMessagesState.h"
#include "register/RegisterController.h"
#include "register/RegisterState.h"
#include "sendFriendInvitation/SendFriendInvitationController.h"
#include "sendFriendInvitation/SendFriendInvitationState.h"
#include "userSettings/UserSettingsController.h"
#include "userSettings/UserSettingsState.h"
#include "verifyUser/VerifyUserController.h"
#include "verifyUser/VerifyUserState.h"

namespace client::gui
{
StateFactory::StateFactory(std::shared_ptr<client::api::Session> sessionInit,
                           std::shared_ptr<StateMachine> stateMachineInit,
                           std::shared_ptr<LoaderController> loaderControllerInit)
    : session{std::move(sessionInit)},
      stateMachine{std::move(stateMachineInit)},
      loaderController{std::move(loaderControllerInit)},
      privateMessagesConversationStorage{std::make_shared<storage::ConversationStorage>()},
      channelConversationStorage{std::make_shared<storage::ConversationStorage>()}
{
}

std::shared_ptr<State> StateFactory::createDefaultState() const
{
    return createLoginState();
}

std::shared_ptr<State> StateFactory::createRegisterState() const
{
    auto controller = std::make_unique<RegisterController>(session, *this, stateMachine);

    return std::make_shared<RegisterState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createLoginState() const
{
    auto controller = std::make_unique<LoginController>(session, *this, stateMachine);

    return std::make_shared<LoginState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createCreateChannelState() const
{
    auto controller = std::make_unique<CreateChannelController>(session, *this, stateMachine);

    return std::make_shared<CreateChannelState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createSendFriendInvitationState() const
{
    auto controller = std::make_unique<SendFriendInvitationController>(session, *this, stateMachine);

    return std::make_shared<SendFriendInvitationState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createUserSettingsState() const
{
    auto controller = std::make_unique<UserSettingsController>(session, *this, stateMachine);

    return std::make_shared<UserSettingsState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createVerifyUserState() const
{
    auto controller = std::make_unique<VerifyUserController>(session, *this, stateMachine);

    return std::make_shared<VerifyUserState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createInviteToChannelState(const std::string& channelId) const
{
    auto controller = std::make_unique<InviteToChannelController>(session, *this, stateMachine, channelId);

    return std::make_shared<InviteToChannelState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createPrivateMessagesState() const
{
    auto privateMessagesController =
        std::make_unique<PrivateMessagesController>(session, *this, stateMachine, privateMessagesConversationStorage);

    auto leftColumnController = std::make_unique<LeftColumnController>(session, *this, stateMachine);

    auto messagesController = std::make_unique<MessagesController>(session, *this, stateMachine);

    return std::make_shared<PrivateMessagesState>(std::move(privateMessagesController), std::move(leftColumnController),
                                                  std::move(messagesController), loaderController);
}

std::shared_ptr<State> StateFactory::createChannelState(const std::string& channelId, const std::string& channelName,
                                                        bool isOwner) const
{
    auto channelController = std::make_unique<ChannelController>(
        session, *this, stateMachine, channelConversationStorage, channelId, channelName, isOwner);

    auto leftColumnController = std::make_unique<LeftColumnController>(session, *this, stateMachine);

    auto messagesController = std::make_unique<MessagesController>(session, *this, stateMachine);

    return std::make_shared<ChannelState>(std::move(channelController), std::move(leftColumnController),
                                          std::move(messagesController), loaderController);
}
}