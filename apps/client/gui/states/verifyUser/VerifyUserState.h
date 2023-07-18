#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "VerifyUserController.h"

namespace client::gui
{
class VerifyUserState : public State
{
public:
    VerifyUserState(std::unique_ptr<VerifyUserController> registerController,
                    std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"verifyUserController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/verifyUser/VerifyUserView.qml")};
    std::unique_ptr<VerifyUserController> verifyUserController;
    std::shared_ptr<LoaderController> loaderController;
};
}
