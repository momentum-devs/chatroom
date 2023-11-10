#include "GroupMapperImpl.h"

#include "gtest/gtest.h"

#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class GroupMapperTest : public Test
{
public:
    GroupTestFactory groupTestFactory;

    GroupMapperImpl groupMapper;
};

TEST_F(GroupMapperTest, givenPersistenceGroup_shouldMapToDomainGroup)
{
    const auto group = groupTestFactory.createPersistentGroup();

    const auto domainGroup = groupMapper.mapToDomainGroup(group);

    ASSERT_EQ(domainGroup->getId(), group->getId());
    ASSERT_EQ(domainGroup->getCreatedAt(), group->getCreatedAt());
}

TEST_F(GroupMapperTest, givenDomainGroup_shouldMapToPersistenceGroup)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getCreatedAt(), user->getUpdatedAt());

    const auto domainGroup = groupTestFactory.createDomainGroup(domainUser);

    EXPECT_CALL(*userMapper, mapToPersistenceUser(domainUser)).WillOnce(Return(user));

    const auto persistenceGroup = groupMapper.mapToPersistenceGroup(domainGroup);

    ASSERT_EQ(persistenceGroup->getId(), domainGroup->getId());
    ASSERT_EQ(persistenceGroup->getName(), domainGroup->getName());
    ASSERT_EQ(persistenceGroup->getCreator(), user);
    ASSERT_EQ(persistenceGroup->getCreatedAt(), domainGroup->getCreatedAt());
    ASSERT_EQ(persistenceGroup->getUpdatedAt(), domainGroup->getUpdatedAt());
}
