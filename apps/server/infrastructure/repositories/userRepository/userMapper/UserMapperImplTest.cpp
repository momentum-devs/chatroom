#include "UserMapperImpl.h"

#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class UserMapperTest : public Test
{
public:
    UserMapperImpl userMapper;
};

TEST_F(UserMapperTest, givenPersistenceUser_shouldMapToDomainUser)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto persistenceUser = std::make_shared<User>(userId, email, password, nickname, active, emailVerified,
                                                        verificationCode, createdAt, updatedAt);

    const auto domainUser = userMapper.mapToDomainUser(persistenceUser);

    ASSERT_EQ(domainUser->getId(), userId);
    ASSERT_EQ(domainUser->getEmail(), email);
    ASSERT_EQ(domainUser->getPassword(), password);
    ASSERT_EQ(domainUser->getNickname(), nickname);
    ASSERT_EQ(domainUser->isActive(), active);
    ASSERT_EQ(domainUser->isEmailVerified(), emailVerified);
    ASSERT_EQ(domainUser->getVerificationCode(), verificationCode);
    ASSERT_EQ(domainUser->getCreatedAt(), createdAt);
    ASSERT_EQ(domainUser->getUpdatedAt(), updatedAt);
}

TEST_F(UserMapperTest, givenDomainUser_shouldMapToPersistenceUser)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto domainUser = std::make_shared<domain::User>(userId, email, password, nickname, active, emailVerified,
                                                           verificationCode, createdAt, updatedAt);

    const auto persistenceUser = userMapper.mapToPersistenceUser(domainUser);

    ASSERT_EQ(persistenceUser->getId(), userId);
    ASSERT_EQ(persistenceUser->getEmail(), email);
    ASSERT_EQ(persistenceUser->getPassword(), password);
    ASSERT_EQ(persistenceUser->getNickname(), nickname);
    ASSERT_EQ(persistenceUser->isActive(), active);
    ASSERT_EQ(persistenceUser->isEmailVerified(), emailVerified);
    ASSERT_EQ(persistenceUser->getVerificationCode(), verificationCode);
    ASSERT_EQ(persistenceUser->getCreatedAt(), createdAt);
    ASSERT_EQ(persistenceUser->getUpdatedAt(), updatedAt);
}
