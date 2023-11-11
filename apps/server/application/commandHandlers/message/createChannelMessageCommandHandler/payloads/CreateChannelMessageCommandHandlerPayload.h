#pragma once

#include <string>

namespace server::application
{
struct CreateChannelMessageCommandHandlerPayload
{
    const std::string content;
    const std::string senderId;
    const std::string channelId;
};

inline bool operator==(const CreateChannelMessageCommandHandlerPayload& lhs,
                       const CreateChannelMessageCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateChannelMessageCommandHandlerPayload& payload)
    { return std::tie(payload.content, payload.senderId, payload.channelId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
