#pragma once

#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"
#include "UserChannelMapper.h"

namespace server::infrastructure
{
class UserChannelMapperImpl : public UserChannelMapper
{
public:
    UserChannelMapperImpl(std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>);

    domain::UserChannel mapToDomainUserChannel(const UserChannel&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
};
}
