#pragma once

#include <stdexcept>

namespace server::application::errors
{
struct ResourceNotFoundError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
