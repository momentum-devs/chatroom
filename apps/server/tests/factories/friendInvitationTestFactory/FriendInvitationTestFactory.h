#pragma once

#include <memory>

#include "Channel.h"
#include "FriendInvitation.h"
#include "server/domain/entities/friendInvitation/FriendInvitation.h"
#include "server/domain/entities/user/User.h"
#include "server/domain/entities/userChannel/UserChannel.h"
#include "User.h"

namespace server::tests
{
class FriendInvitationTestFactory
{
public:
    static std::shared_ptr<domain::FriendInvitation>
    createDomainFriendInvitation(const std::shared_ptr<domain::User>& sender,
                                 const std::shared_ptr<domain::User>& recipient);
    static std::shared_ptr<infrastructure::FriendInvitation>
    createPersistentFriendInvitation(const std::shared_ptr<infrastructure::User>& sender,
                                     const std::shared_ptr<infrastructure::User>& recipient);
};
}
