#include "ReactionMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
ReactionMapperImpl::ReactionMapperImpl(std::shared_ptr<UserMapper> userMapperInit,
                                       std::shared_ptr<MessageMapper> messageMapperInit)
    : userMapper{std::move(userMapperInit)}, messageMapper{std::move(messageMapperInit)}
{
}

domain::Reaction ReactionMapperImpl::mapToDomainReaction(const Reaction& reaction) const
{
    const auto id = reaction.getId();
    const auto name = reaction.getName();
    const auto user = userMapper->mapToDomainUser(reaction.getUser());
    const auto message = messageMapper->mapToDomainMessage(reaction.getMessage());

    return domain::Reaction{id, name, user, message};
}
}
