#include "FriendshipMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class FriendshipMapperTest : public Test
{
public:
    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();

    FriendshipMapperImpl channelInvitationMapper{userMapper};
};

TEST_F(FriendshipMapperTest, givenPersistenceFriendship_shouldMapToDomainFriendship)
{
    const auto userId1 = faker::String::uuid();
    const auto userId2 = faker::String::uuid();
    const auto email1 = faker::Internet::email();
    const auto email2 = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = std::make_shared<User>(userId1, email1, password, nickname, active, emailVerified,
                                             verificationCode, createdAt, updatedAt);

    const auto domainUser = std::make_shared<domain::User>(userId1, email1, password, nickname, active, emailVerified,
                                                           verificationCode, createdAt, updatedAt);

    const auto userFriend = std::make_shared<User>(userId2, email2, password, nickname, active, emailVerified,
                                                   verificationCode, createdAt, updatedAt);

    const auto domainUserFriend = std::make_shared<domain::User>(userId2, email2, password, nickname, active,
                                                                 emailVerified, verificationCode, createdAt, updatedAt);

    const auto id = faker::String::uuid();

    Friendship persistenceFriendship{id, user, userFriend, createdAt, updatedAt};

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));
    EXPECT_CALL(*userMapper, mapToDomainUser(userFriend)).WillOnce(Return(domainUserFriend));

    const auto domainFriendship = channelInvitationMapper.mapToDomainFriendship(persistenceFriendship);

    ASSERT_EQ(domainFriendship.getId(), id);
    ASSERT_EQ(domainFriendship.getUser(), domainUser);
    ASSERT_EQ(domainFriendship.getUserFriend(), domainUserFriend);
    ASSERT_EQ(domainFriendship.getCreatedAt(), createdAt);
    ASSERT_EQ(domainFriendship.getUpdatedAt(), updatedAt);
}
