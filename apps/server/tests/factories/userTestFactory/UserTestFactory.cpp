#include "UserTestFactory.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::User> UserTestFactory::createDomainUser()
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<domain::User>(id, email, password, email, active, emailVerified, verificationCode,
                                          createdAt, updatedAt);
}

std::shared_ptr<infrastructure::User> UserTestFactory::createPersistentUser()
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<infrastructure::User>(id, email, password, email, active, emailVerified, verificationCode,
                                          createdAt, updatedAt);
}
}
