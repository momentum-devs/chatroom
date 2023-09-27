#pragma once

#include <string>

namespace server::application
{
struct FindUserByEmailQueryHandlerPayload
{
    const std::string email;
};

inline bool operator==(const FindUserByEmailQueryHandlerPayload& lhs, const FindUserByEmailQueryHandlerPayload& rhs)
{
    return lhs.email == rhs.email;
}
}
