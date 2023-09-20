#pragma once

#include <stdexcept>

namespace common::messages
{
struct InvalidChecksumError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
