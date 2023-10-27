#pragma once

#include <stdexcept>

namespace server::application::errors
{
struct InvalidTokenError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
