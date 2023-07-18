#include "StateMachine.h"

namespace client::gui
{
void StateMachine::addNextState(std::shared_ptr<State> state)
{
    if (not states.empty())
    {
        states.top()->deactivate();
    }

    states.push(std::move(state));

    states.top()->activate();
}

void StateMachine::returnToThePreviousState()
{
    if (states.empty())
    {
        // TODO: throw exception
        return;
    }

    states.top()->deactivate();

    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void StateMachine::clear(std::optional<std::shared_ptr<State>> state)
{
    if (not states.empty())
    {
        states.top()->deactivate();
    }

    while (not states.empty())
    {
        states.pop();
    }

    if (state.has_value())
    {
        addNextState(state.value());
    }
}
}