#include "UserMapperImpl.h"

#include "gtest/gtest.h"

#include "../../../../tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class UserMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;

    UserMapperImpl userMapper;
};

TEST_F(UserMapperTest, givenPersistenceUser_shouldMapToDomainUser)
{
    const auto persistenceUser = userTestFactory.createPersistentUser();

    const auto domainUser = userMapper.mapToDomainUser(persistenceUser);

    ASSERT_EQ(domainUser->getId(), persistenceUser->getId());
    ASSERT_EQ(domainUser->getEmail(), persistenceUser->getEmail());
    ASSERT_EQ(domainUser->getPassword(), persistenceUser->getPassword());
    ASSERT_EQ(domainUser->getNickname(), persistenceUser->getNickname());
    ASSERT_EQ(domainUser->isActive(), persistenceUser->isActive());
    ASSERT_EQ(domainUser->isEmailVerified(), persistenceUser->isEmailVerified());
    ASSERT_EQ(domainUser->getVerificationCode(), persistenceUser->getVerificationCode());
    ASSERT_EQ(domainUser->getResetPasswordCode(), persistenceUser->getResetPasswordCode());
    ASSERT_EQ(domainUser->getCreatedAt(), persistenceUser->getCreatedAt());
    ASSERT_EQ(domainUser->getUpdatedAt(), persistenceUser->getUpdatedAt());
    ASSERT_EQ(domainUser->getAvatarUrl(), persistenceUser->getAvatarUrl().get());
}

TEST_F(UserMapperTest, givenDomainUser_shouldMapToPersistenceUser)
{
    const auto domainUser = userTestFactory.createDomainUser();

    const auto persistenceUser = userMapper.mapToPersistenceUser(domainUser);

    ASSERT_EQ(persistenceUser->getId(), domainUser->getId());
    ASSERT_EQ(persistenceUser->getEmail(), domainUser->getEmail());
    ASSERT_EQ(persistenceUser->getPassword(), domainUser->getPassword());
    ASSERT_EQ(persistenceUser->getNickname(), domainUser->getNickname());
    ASSERT_EQ(persistenceUser->isActive(), domainUser->isActive());
    ASSERT_EQ(persistenceUser->isEmailVerified(), domainUser->isEmailVerified());
    ASSERT_EQ(persistenceUser->getVerificationCode(), domainUser->getVerificationCode());
    ASSERT_EQ(persistenceUser->getResetPasswordCode().get(), domainUser->getResetPasswordCode());
    ASSERT_EQ(persistenceUser->getCreatedAt(), domainUser->getCreatedAt());
    ASSERT_EQ(persistenceUser->getUpdatedAt(), domainUser->getUpdatedAt());
    ASSERT_EQ(persistenceUser->getAvatarUrl().get(), domainUser->getAvatarUrl());
}
