#include "FriendshipTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::Friendship>
FriendshipTestFactory::createDomainFriendship(const std::shared_ptr<domain::User>& user,
                                              const std::shared_ptr<domain::User>& userFriend)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<domain::Friendship>(id, user, userFriend, createdAt);
}

std::shared_ptr<infrastructure::Friendship>
FriendshipTestFactory::createPersistentFriendship(const std::shared_ptr<infrastructure::User>& user,
                                                  const std::shared_ptr<infrastructure::User>& userFriend)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<infrastructure::Friendship>(id, user, userFriend, createdAt);
}
}
