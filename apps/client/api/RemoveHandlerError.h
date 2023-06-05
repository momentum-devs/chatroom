#pragma once

#include <stdexcept>

namespace client::api
{
struct RemoveHandlerError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}