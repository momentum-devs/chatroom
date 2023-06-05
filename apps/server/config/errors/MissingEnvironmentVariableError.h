#pragma once

#include <stdexcept>

namespace server::config::errors
{
struct MissingEnvironmentVariableError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
