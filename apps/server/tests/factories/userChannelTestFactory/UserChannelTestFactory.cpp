#include "UserChannelTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::UserChannel>
UserChannelTestFactory::createDomainUserChannel(const std::shared_ptr<domain::User>& user,
                                                const std::shared_ptr<domain::Channel>& channel)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<domain::UserChannel>(id, user, channel, createdAt, updatedAt);
}

std::shared_ptr<infrastructure::UserChannel>
UserChannelTestFactory::createPersistentUserChannel(const std::shared_ptr<infrastructure::User>& user,
                                                    const std::shared_ptr<infrastructure::Channel>& channel)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<infrastructure::UserChannel>(id, user, channel, createdAt, updatedAt);
}
}
