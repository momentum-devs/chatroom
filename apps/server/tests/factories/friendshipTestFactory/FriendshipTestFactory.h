#pragma once

#include <memory>

#include "Channel.h"
#include "Friendship.h"
#include "server/domain/entities/friendship/Friendship.h"
#include "server/domain/entities/user/User.h"
#include "server/domain/entities/userChannel/UserChannel.h"
#include "User.h"

namespace server::tests
{
class FriendshipTestFactory
{
public:
    std::shared_ptr<domain::Friendship> createDomainFriendship(const std::shared_ptr<domain::User>& user,
                                                               const std::shared_ptr<domain::User>& userFriend,
                                                               const std::shared_ptr<domain::Group>& group);
    std::shared_ptr<infrastructure::Friendship>
    createPersistentFriendship(const std::shared_ptr<infrastructure::User>& user,
                               const std::shared_ptr<infrastructure::User>& userFriend,
                               const std::shared_ptr<infrastructure::Group>& group);
};
}
