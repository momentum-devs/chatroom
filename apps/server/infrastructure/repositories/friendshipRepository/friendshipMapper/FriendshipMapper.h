#pragma once

#include "../../../../domain/entities/friendship/Friendship.h"
#include "../../../database/entities/Friendship.h"

namespace server::infrastructure
{
class FriendshipMapper
{
public:
    virtual ~FriendshipMapper() = default;

    virtual domain::Friendship mapToDomainFriendship(const Friendship&) const = 0;
};
}
