#pragma once

#include <optional>
#include <string>

namespace server::application
{
struct UploadUserAvatarCommandHandlerPayload
{
    std::string id;
    std::string data;
};

inline bool operator==(const UploadUserAvatarCommandHandlerPayload& lhs,
                       const UploadUserAvatarCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const UploadUserAvatarCommandHandlerPayload& payload)
    { return std::tie(payload.id, payload.data); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
