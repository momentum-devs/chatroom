#pragma once

#include "ConversationMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ConversationMapperImpl : public ConversationMapper
{
public:
    ConversationMapperImpl(std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>);

    domain::Conversation mapToDomainConversation(const Conversation&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
};
}
