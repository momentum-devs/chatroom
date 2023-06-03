#pragma once

#include <stdexcept>

namespace server::application
{
struct UserNotFoundError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
