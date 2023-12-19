#include "UserGroupMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperMock.h"

#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/userGroupTestFactory/UserGroupTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class UserGroupMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    GroupTestFactory groupTestFactory;
    UserGroupTestFactory userGroupTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<GroupMapperMock> groupMapper = std::make_shared<StrictMock<GroupMapperMock>>();

    UserGroupMapperImpl userGroupMapper{userMapper, groupMapper};
};

TEST_F(UserGroupMapperTest, givenPersistenceUserGroup_shouldMapToDomainUserGroup)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getCreatedAt(), user->getUpdatedAt(),
        user->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : user->getAvatarUrl().get());

    const auto group = groupTestFactory.createPersistentGroup();

    const auto domainGroup = std::make_shared<domain::Group>(group->getId(), group->getCreatedAt());

    const auto userGroup = userGroupTestFactory.createPersistentUserGroup(user, group);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    EXPECT_CALL(*groupMapper, mapToDomainGroup(group)).WillOnce(Return(domainGroup));

    const auto domainUserGroup = userGroupMapper.mapToDomainUserGroup(*userGroup);

    ASSERT_EQ(domainUserGroup.getId(), userGroup->getId());
    ASSERT_EQ(domainUserGroup.getLastReadMessageId(), userGroup->getLastReadMessageId().get());
    ASSERT_EQ(domainUserGroup.getUser(), domainUser);
    ASSERT_EQ(domainUserGroup.getGroup(), domainGroup);
    ASSERT_EQ(domainUserGroup.getCreatedAt(), userGroup->getCreatedAt());
}
