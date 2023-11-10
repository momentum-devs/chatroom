#include "StateFactory.h"

#include "channelMembers/ChannelMembersController.h"
#include "channelMembers/ChannelMembersState.h"
#include "createChannel/CreateChannelController.h"
#include "createChannel/CreateChannelState.h"
#include "gui/controllers/LeftColumnController.h"
#include "inviteToChannel/InviteToChannelController.h"
#include "inviteToChannel/InviteToChannelState.h"
#include "login/LoginController.h"
#include "login/LoginState.h"
#include "main/MainController.h"
#include "main/MainState.h"
#include "privateMessages/PrivateMessagesState.h"
#include "register/RegisterController.h"
#include "register/RegisterState.h"
#include "sendFriendRequest/SendFriendRequestController.h"
#include "sendFriendRequest/SendFriendRequestState.h"
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
      loaderController{std::move(loaderControllerInit)}
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

std::shared_ptr<State> StateFactory::createMainState() const
{
    auto controller = std::make_unique<MainController>(session, *this, stateMachine);

    return std::make_shared<MainState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createCreateChannelState() const
{
    auto controller = std::make_unique<CreateChannelController>(session, *this, stateMachine);

    return std::make_shared<CreateChannelState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createSendFriendRequestState() const
{
    auto controller = std::make_unique<SendFriendRequestController>(session, *this, stateMachine);

    return std::make_shared<SendFriendRequestState>(std::move(controller), loaderController);
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

std::shared_ptr<State> StateFactory::createChannelMembersListState(const std::string& channelId) const
{
    auto controller = std::make_unique<ChannelMembersController>(session, *this, stateMachine, channelId);

    return std::make_shared<ChannelMembersState>(std::move(controller), loaderController);
}

std::shared_ptr<State> StateFactory::createPrivateMessagesState() const
{
    auto leftColumnController = std::make_unique<LeftColumnController>(session, *this, stateMachine);

    return std::make_shared<PrivateMessagesState>(std::move(leftColumnController), loaderController);
}
}