#pragma once

#include <vector>

#include "../../../../../domain/entities/friendInvitation/FriendInvitation.h"

namespace server::application
{
struct FindReceivedFriendInvitationsQueryHandlerResult
{
    std::vector<domain::FriendInvitation> friendInvitations;
};
}
