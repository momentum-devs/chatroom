#include "FriendshipMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"

#include "server/tests/factories/friendshipTestFactory/FriendshipTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class FriendshipMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;

    FriendshipTestFactory friendshipTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();

    FriendshipMapperImpl channelInvitationMapper{userMapper};
};

TEST_F(FriendshipMapperTest, givenPersistenceFriendship_shouldMapToDomainFriendship)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getCreatedAt(), user->getUpdatedAt());

    const auto userFriend = userTestFactory.createPersistentUser();

    const auto domainUserFriend = std::make_shared<domain::User>(
        userFriend->getId(), userFriend->getEmail(), userFriend->getPassword(), userFriend->getNickname(),
        userFriend->isActive(), userFriend->isEmailVerified(), userFriend->getVerificationCode(),
        userFriend->getCreatedAt(), userFriend->getUpdatedAt());

    const auto friendship = friendshipTestFactory.createPersistentFriendship(user, userFriend);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));
    EXPECT_CALL(*userMapper, mapToDomainUser(userFriend)).WillOnce(Return(domainUserFriend));

    const auto domainFriendship = channelInvitationMapper.mapToDomainFriendship(*friendship);

    ASSERT_EQ(domainFriendship.getId(), friendship->getId());
    ASSERT_EQ(domainFriendship.getUser(), domainUser);
    ASSERT_EQ(domainFriendship.getUserFriend(), domainUserFriend);
    ASSERT_EQ(domainFriendship.getCreatedAt(), friendship->getCreatedAt());
}
