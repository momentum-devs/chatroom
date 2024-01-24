#pragma once

#include <string>

namespace server::application
{
struct UploadChannelAvatarCommandHandlerPayload
{
    std::string id;
    std::string avatarData;
};

inline bool operator==(const UploadChannelAvatarCommandHandlerPayload& lhs,
                       const UploadChannelAvatarCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const UploadChannelAvatarCommandHandlerPayload& payload)
    { return std::tie(payload.id, payload.avatarData); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
