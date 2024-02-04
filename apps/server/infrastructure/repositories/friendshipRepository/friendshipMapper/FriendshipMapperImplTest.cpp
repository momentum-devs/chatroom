#include "FriendshipMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperMock.h"

#include "server/tests/factories/friendshipTestFactory/FriendshipTestFactory.h"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class FriendshipMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    GroupTestFactory groupTestFactory;
    FriendshipTestFactory friendshipTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();

    std::shared_ptr<GroupMapperMock> groupMapper = std::make_shared<StrictMock<GroupMapperMock>>();

    FriendshipMapperImpl channelInvitationMapper{userMapper, groupMapper};
};

TEST_F(FriendshipMapperTest, givenPersistenceFriendship_shouldMapToDomainFriendship)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getResetPasswordCode().get(), user->getCreatedAt(),
        user->getUpdatedAt(),
        user->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : user->getAvatarUrl().get());

    const auto userFriend = userTestFactory.createPersistentUser();

    const auto domainUserFriend = std::make_shared<domain::User>(
        userFriend->getId(), userFriend->getEmail(), userFriend->getPassword(), userFriend->getNickname(),
        userFriend->isActive(), userFriend->isEmailVerified(), userFriend->getVerificationCode(),
        user->getResetPasswordCode().get(), userFriend->getCreatedAt(), userFriend->getUpdatedAt(),
        userFriend->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) :
                                            userFriend->getAvatarUrl().get());

    const auto group = groupTestFactory.createPersistentGroup();

    const auto domainGroup = std::make_shared<domain::Group>(group->getId(), group->getCreatedAt());

    const auto friendship = friendshipTestFactory.createPersistentFriendship(user, userFriend, group);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));
    EXPECT_CALL(*userMapper, mapToDomainUser(userFriend)).WillOnce(Return(domainUserFriend));
    EXPECT_CALL(*groupMapper, mapToDomainGroup(group)).WillOnce(Return(domainGroup));

    const auto domainFriendship = channelInvitationMapper.mapToDomainFriendship(*friendship);

    ASSERT_EQ(domainFriendship.getId(), friendship->getId());
    ASSERT_EQ(domainFriendship.getUser(), domainUser);
    ASSERT_EQ(domainFriendship.getUserFriend(), domainUserFriend);
    ASSERT_EQ(domainFriendship.getGroup(), domainGroup);
    ASSERT_EQ(domainFriendship.getCreatedAt(), friendship->getCreatedAt());
}
