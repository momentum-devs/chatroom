#include "ChannelTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"

namespace server::tests
{
std::shared_ptr<domain::Channel> ChannelTestFactory::createDomainChannel(const std::shared_ptr<domain::User>& user)
{
    const auto id = faker::String::uuid();
    const auto name = faker::Word::noun();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<domain::Channel>(id, name, user, createdAt, updatedAt);
}

std::shared_ptr<infrastructure::Channel>
ChannelTestFactory::createPersistentChannel(const std::shared_ptr<infrastructure::User>& user)
{
    const auto id = faker::String::uuid();
    const auto name = faker::Word::noun();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<infrastructure::Channel>(id, name, user, createdAt, updatedAt);
}
}
