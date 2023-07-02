#pragma once

#include <stdexcept>

namespace server::application::errors
{
struct ResourceAlreadyExistsError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
