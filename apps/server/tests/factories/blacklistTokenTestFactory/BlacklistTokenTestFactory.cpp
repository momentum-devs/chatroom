#include "BlacklistTokenTestFactory.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::BlacklistToken> BlacklistTokenTestFactory::createDomainBlacklistToken()
{
    const auto id = faker::String::uuid();
    const auto token = faker::String::alphanumeric(32);
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<domain::BlacklistToken>(id, token, createdAt);
}

std::shared_ptr<infrastructure::BlacklistToken> BlacklistTokenTestFactory::createPersistentBlacklistToken()
{
    const auto id = faker::String::uuid();
    const auto token = faker::String::alphanumeric(32);
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<infrastructure::BlacklistToken>(id, token, createdAt);
}
}
