#include "UserTestFactory.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::User> UserTestFactory::createDomainUser(std::size_t nicknameMaxLength)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password(16, {true, true, true, true});
    auto nickname = faker::Internet::username();
    if (nickname.size() > nicknameMaxLength)
    {
        nickname = nickname.substr(0, nicknameMaxLength);
    }
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    return std::make_shared<domain::User>(id, email, password, nickname, active, emailVerified, verificationCode,
                                          resetPasswordCode, createdAt, updatedAt, avatarUrl);
}

std::shared_ptr<infrastructure::User> UserTestFactory::createPersistentUser()
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password(16, {true, true, true, true});
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    return std::make_shared<infrastructure::User>(id, email, password, email, active, emailVerified, verificationCode,
                                                  resetPasswordCode, createdAt, updatedAt, avatarUrl);
}
}
