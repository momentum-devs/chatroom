#pragma once

#include <string>

namespace server::application
{
struct DeleteUserCommandHandlerPayload
{
    const std::string id;
};

inline bool operator==(const DeleteUserCommandHandlerPayload& lhs, const DeleteUserCommandHandlerPayload& rhs)
{
    return lhs.id == rhs.id;
}
}
