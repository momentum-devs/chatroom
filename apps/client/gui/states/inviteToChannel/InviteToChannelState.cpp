#include "InviteToChannelState.h"

#include <loguru.hpp>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
InviteToChannelState::InviteToChannelState(std::unique_ptr<InviteToChannelController> inviteToChannelControllerInit,
                                           std::shared_ptr<LoaderController> loaderControllerInit)
    : inviteToChannelController{std::move(inviteToChannelControllerInit)},
      loaderController{std::move(loaderControllerInit)}
{
}

void InviteToChannelState::activate()
{
    LOG_S(INFO) << "Activate InviteToChannelState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, inviteToChannelController.get());

    loaderController->callLoadView(qUrl);

    inviteToChannelController->activate();
}

void InviteToChannelState::deactivate()
{
    LOG_S(INFO) << "Deactivate InviteToChannelState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    inviteToChannelController->deactivate();
}
}