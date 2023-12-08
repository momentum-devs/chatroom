#pragma once

#include "MessageMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class MessageMapperImpl : public MessageMapper
{
public:
    MessageMapperImpl(std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>, std::shared_ptr<GroupMapper>);

    std::shared_ptr<domain::Message> mapToDomainMessage(std::shared_ptr<Message>) const override;
    std::shared_ptr<Message> mapToPersistenceMessage(const std::shared_ptr<domain::Message>) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
    std::shared_ptr<GroupMapper> groupMapper;
};
}
