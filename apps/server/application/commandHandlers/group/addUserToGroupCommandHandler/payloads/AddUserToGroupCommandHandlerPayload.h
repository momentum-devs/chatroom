#pragma once

#include <string>

namespace server::application
{
struct AddUserToGroupCommandHandlerPayload
{
    const std::string userId;
    const std::string groupId;
};

inline bool operator==(const AddUserToGroupCommandHandlerPayload& lhs, const AddUserToGroupCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const AddUserToGroupCommandHandlerPayload& payload)
    { return std::tie(payload.userId, payload.groupId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
