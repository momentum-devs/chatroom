#pragma once

#include "ReactionMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ReactionMapperImpl : public ReactionMapper
{
public:
    ReactionMapperImpl(std::shared_ptr<UserMapper>, std::shared_ptr<MessageMapper>);

    domain::Reaction mapToDomainReaction(const Reaction&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<MessageMapper> messageMapper;
};
}
