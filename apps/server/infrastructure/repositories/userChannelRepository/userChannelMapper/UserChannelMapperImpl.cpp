#include "UserChannelMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
UserChannelMapperImpl::UserChannelMapperImpl(std::shared_ptr<UserMapper> userMapperInit,
                                             std::shared_ptr<ChannelMapper> channelMapperInit)
    : userMapper{std::move(userMapperInit)}, channelMapper{std::move(channelMapperInit)}
{
}

domain::UserChannel UserChannelMapperImpl::mapToDomainUserChannel(const UserChannel& userChannel) const
{
    const auto id = userChannel.getId();
    const auto user = userMapper->mapToDomainUser(userChannel.getUser());
    const auto channel = channelMapper->mapToDomainChannel(userChannel.getChannel());
    const auto createdAt = userChannel.getCreatedAt();

    return domain::UserChannel{id, user, channel, createdAt};
}
}
