#pragma once

#include <stdexcept>

namespace server::infrastructure::errors
{
struct ReactionRepositoryError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
