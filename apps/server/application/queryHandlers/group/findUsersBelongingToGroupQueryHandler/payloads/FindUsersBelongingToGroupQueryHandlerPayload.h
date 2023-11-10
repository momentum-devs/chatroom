#pragma once

#include <string>

namespace server::application
{
struct FindUsersBelongingToGroupQueryHandlerPayload
{
    const std::string groupId;
};

inline bool operator==(const FindUsersBelongingToGroupQueryHandlerPayload& lhs,
                       const FindUsersBelongingToGroupQueryHandlerPayload& rhs)
{
    return lhs.groupId == rhs.groupId;
}
}
