#pragma once

#include <stdexcept>

namespace common::utils::errors
{
struct FileNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
