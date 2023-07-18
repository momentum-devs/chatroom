#pragma once

#include <memory>
#include <optional>
#include <stack>

#include "State.h"

namespace client::gui
{
class StateMachine
{
public:
    void addNextState(std::shared_ptr<State> state);
    void returnToThePreviousState();
    void clear(std::optional<std::shared_ptr<State>> state = std::nullopt);

private:
    std::stack<std::shared_ptr<State>> states;
};
}