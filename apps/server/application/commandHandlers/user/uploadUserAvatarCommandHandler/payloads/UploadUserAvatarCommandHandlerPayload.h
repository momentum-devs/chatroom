#pragma once

#include <string>

namespace server::application
{
struct UploadUserAvatarCommandHandlerPayload
{
    std::string id;
    std::string avatarName;
    std::string avatarData;
};

inline bool operator==(const UploadUserAvatarCommandHandlerPayload& lhs,
                       const UploadUserAvatarCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const UploadUserAvatarCommandHandlerPayload& payload)
    { return std::tie(payload.id, payload.avatarName, payload.avatarData); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
