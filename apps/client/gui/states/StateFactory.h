#pragma once

#include <QQuickView>

#include "client/api/Session.h"
#include "State.h"
#include "StateMachine.h"

namespace client::gui
{
class StateFactory
{
public:
    StateFactory(std::shared_ptr<client::api::Session> session, std::shared_ptr<StateMachine> stateMachine,
                 std::shared_ptr<QQuickView> view);
    std::shared_ptr<State> createDefaultState() const;
    std::shared_ptr<State> createRegisterState() const;

private:
    std::shared_ptr<client::api::Session> session;
    std::shared_ptr<StateMachine> stateMachine;
    std::shared_ptr<QQuickView> view;
};
}