#pragma once

#include "../../../../domain/entities/friendInvitation/FriendInvitation.h"
#include "../../../database/entities/FriendInvitation.h"

namespace server::infrastructure
{
class FriendInvitationMapper
{
public:
    virtual ~FriendInvitationMapper() = default;

    virtual domain::FriendInvitation mapToDomainFriendInvitation(const FriendInvitation&) const = 0;
};
}
