#include "UserChannelMapperImpl.h"

namespace server::infrastructure
{
domain::UserChannel UserChannelMapperImpl::mapToDomainUserChannel(const UserChannel& userChannel) const
{
    const auto id = userChannel.getId();
    const auto userId = userChannel.getUserId();
    const auto channelId = userChannel.getChannelId();
    const auto createdAt = userChannel.getCreatedAt();
    const auto updatedAt = userChannel.getUpdatedAt();

    return domain::UserChannel{id, userId, channelId, createdAt, updatedAt};
}
}
