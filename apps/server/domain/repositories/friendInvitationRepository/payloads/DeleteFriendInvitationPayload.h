#pragma once

#include <string>

#include "../../../entities/friendInvitation/FriendInvitation.h"

namespace server::domain
{
struct DeleteFriendInvitationPayload
{
    FriendInvitation friendInvitation;
};
}
