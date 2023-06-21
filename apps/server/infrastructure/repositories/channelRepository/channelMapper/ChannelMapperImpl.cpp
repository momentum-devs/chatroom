#include "ChannelMapperImpl.h"

namespace server::infrastructure
{
domain::Channel ChannelMapperImpl::mapToDomainChannel(const Channel& user) const
{
    const auto id = user.getId();
    const auto email = user.getEmail();
    const auto password = user.getPassword();
    const auto nickname = user.getNickname();
    const auto createdAt = user.getCreatedAt();
    const auto updatedAt = user.getUpdatedAt();

    return domain::Channel{id, email, password, nickname, createdAt, updatedAt};
}
}
