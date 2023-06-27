#pragma once

#include <memory>
#include <stack>

#include "State.h"

namespace client::gui
{
class StateMachine
{
public:
    void addNextState(std::shared_ptr<State> state);
    void returnToThePreviousState();
    void setNewRootState(std::shared_ptr<State> state);

private:
    std::stack<std::shared_ptr<State>> states;
};
}