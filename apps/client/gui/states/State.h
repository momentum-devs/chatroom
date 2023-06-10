#pragma once

namespace client::gui
{
class State
{
public:
    virtual ~State() = default;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};
}