#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "MainController.h"

namespace client::gui
{
class MainState : public State
{
public:
    MainState(std::unique_ptr<MainController> registerController, std::shared_ptr<LoaderController> loaderController);

    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"mainController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/main/MainView.qml")};
    std::unique_ptr<MainController> mainController;
    std::shared_ptr<LoaderController> loaderController;
};
}