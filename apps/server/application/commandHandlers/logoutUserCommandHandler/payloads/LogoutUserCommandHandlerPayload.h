#pragma once

#include <string>

namespace server::application
{
struct LogoutUserCommandHandlerPayload
{
    const std::string id;
};

inline bool operator==(const LogoutUserCommandHandlerPayload& lhs, const LogoutUserCommandHandlerPayload& rhs)
{
    return lhs.id == rhs.id;
}
}
