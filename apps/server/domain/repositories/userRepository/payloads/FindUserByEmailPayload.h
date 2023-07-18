#pragma once

#include <string>

namespace server::domain
{
struct FindUserByEmailPayload
{
    const std::string email;
};

inline bool operator==(const FindUserByEmailPayload& lhs, const FindUserByEmailPayload& rhs)
{
    return lhs.email == rhs.email;
}
}
