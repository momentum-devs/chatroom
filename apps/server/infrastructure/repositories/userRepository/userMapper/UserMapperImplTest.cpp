#include "UserMapperImpl.h"

#include "gtest/gtest.h"

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
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto active = true;
    const auto emailVerified = false;
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto persistenceUser =
        std::make_shared<User>(id, email, password, nickname, active, emailVerified, createdAt, updatedAt);

    const auto domainUser = userMapper.mapToDomainUser(persistenceUser);

    ASSERT_EQ(domainUser->getId(), id);
    ASSERT_EQ(domainUser->getEmail(), email);
    ASSERT_EQ(domainUser->getPassword(), password);
    ASSERT_EQ(domainUser->getNickname(), nickname);
    ASSERT_EQ(domainUser->isActive(), active);
    ASSERT_EQ(domainUser->isEmailVerified(), emailVerified);
    ASSERT_EQ(domainUser->getCreatedAt(), createdAt);
    ASSERT_EQ(domainUser->getUpdatedAt(), updatedAt);
}

TEST_F(UserMapperTest, givenDomainUser_shouldMapToPersistenceUser)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto active = true;
    const auto emailVerified = false;
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto domainUser =
        std::make_shared<domain::User>(id, email, password, nickname, active, emailVerified, createdAt, updatedAt);

    const auto persistenceUser = userMapper.mapToPersistenceUser(domainUser);

    ASSERT_EQ(persistenceUser->getId(), id);
    ASSERT_EQ(persistenceUser->getEmail(), email);
    ASSERT_EQ(persistenceUser->getPassword(), password);
    ASSERT_EQ(persistenceUser->getNickname(), nickname);
    ASSERT_EQ(persistenceUser->isActive(), active);
    ASSERT_EQ(persistenceUser->isEmailVerified(), emailVerified);
    ASSERT_EQ(persistenceUser->getCreatedAt(), createdAt);
    ASSERT_EQ(persistenceUser->getUpdatedAt(), updatedAt);
}
