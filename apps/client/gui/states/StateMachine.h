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

private:
    std::stack<std::shared_ptr<State>> states;
};
}