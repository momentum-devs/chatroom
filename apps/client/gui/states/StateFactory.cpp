#include "StateFactory.h"

#include "createChannel/CreateChannelController.h"
#include "createChannel/CreateChannelState.h"
#include "login/LoginController.h"
#include "login/LoginState.h"
#include "main/MainController.h"
#include "main/MainState.h"
#include "register/RegisterController.h"
#include "register/RegisterState.h"

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
}