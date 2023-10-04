#include "FriendInvitationTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::FriendInvitation>
FriendInvitationTestFactory::createDomainFriendInvitation(const std::shared_ptr<domain::User>& sender,
                                                          const std::shared_ptr<domain::User>& recipient)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<domain::FriendInvitation>(id, sender, recipient, createdAt, updatedAt);
}

std::shared_ptr<infrastructure::FriendInvitation>
FriendInvitationTestFactory::createPersistentFriendInvitation(const std::shared_ptr<infrastructure::User>& sender,
                                                              const std::shared_ptr<infrastructure::User>& recipient)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<infrastructure::FriendInvitation>(id, sender, recipient, createdAt, updatedAt);
}
}
