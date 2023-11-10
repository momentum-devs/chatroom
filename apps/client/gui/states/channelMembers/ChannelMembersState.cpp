#include "ChannelMembersState.h"

#include <loguru.hpp>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
ChannelMembersState::ChannelMembersState(std::unique_ptr<ChannelMembersController> channelMembersControllerInit,
                                         std::shared_ptr<LoaderController> loaderControllerInit)
    : channelMembersController{std::move(channelMembersControllerInit)},
      loaderController{std::move(loaderControllerInit)}
{
}

void ChannelMembersState::activate()
{
    LOG_S(INFO) << "Activate ChannelMembersState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, channelMembersController.get());

    loaderController->callLoadView(qUrl);

    channelMembersController->activate();
}

void ChannelMembersState::deactivate()
{
    LOG_S(INFO) << "Deactivate ChannelMembersState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    channelMembersController->deactivate();
}
}