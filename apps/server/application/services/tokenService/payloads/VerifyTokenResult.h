#pragma once

#include <string>

namespace server::application
{
struct VerifyTokenResult
{
    std::string userId;
};

inline bool operator==(const VerifyTokenResult& lhs, const VerifyTokenResult& rhs)
{
    return lhs.userId == rhs.userId;
}
}
