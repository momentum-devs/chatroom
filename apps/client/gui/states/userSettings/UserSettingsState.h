

#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "UserSettingsController.h"

namespace client::gui
{
class UserSettingsState : public State
{
public:
    UserSettingsState(std::unique_ptr<UserSettingsController> registerController,
                      std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"userSettingsController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/userSettings/UserSettingsView.qml")};
    std::unique_ptr<UserSettingsController> userSettingsController;
    std::shared_ptr<LoaderController> loaderController;
};
}
