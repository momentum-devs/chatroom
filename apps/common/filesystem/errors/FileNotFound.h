#pragma once

#include <stdexcept>

namespace common::filesystem::errors
{
struct FileNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
