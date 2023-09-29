#pragma once

#include "ChannelMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ChannelMapperImpl : public ChannelMapper
{
public:
    explicit ChannelMapperImpl(std::shared_ptr<UserMapper>);

    std::shared_ptr<domain::Channel> mapToDomainChannel(std::shared_ptr<Channel>) const override;
    std::shared_ptr<Channel> mapToPersistenceChannel(std::shared_ptr<domain::Channel>) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
};
}
