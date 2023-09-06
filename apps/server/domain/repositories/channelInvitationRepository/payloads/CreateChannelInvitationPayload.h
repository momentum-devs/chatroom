#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateChannelInvitationPayload
{
    const std::string id;
    const std::shared_ptr<User> sender;
    const std::shared_ptr<User> recipient;
    const std::shared_ptr<Channel> channel;
};

inline bool operator==(const CreateChannelInvitationPayload& lhs, const CreateChannelInvitationPayload& rhs)
{
    auto tieStruct = [](const CreateChannelInvitationPayload& payload)
    { return std::tie(payload.id, *payload.sender, *payload.recipient, *payload.channel); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
