#pragma once

#include <stdexcept>

namespace client::config
{
struct MissingEnvironmentVariableError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
