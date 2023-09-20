#include "FriendInvitationMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class FriendInvitationMapperTest : public Test
{
public:
    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();

    FriendInvitationMapperImpl channelInvitationMapper{userMapper};
};

TEST_F(FriendInvitationMapperTest, givenPersistenceFriendInvitation_shouldMapToDomainFriendInvitation)
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

    const auto sender = std::make_shared<User>(userId1, email1, password, nickname, active, emailVerified,
                                               verificationCode, createdAt, updatedAt);

    const auto domainSender = std::make_shared<domain::User>(userId1, email1, password, nickname, active, emailVerified,
                                                             verificationCode, createdAt, updatedAt);

    const auto recipient = std::make_shared<User>(userId2, email2, password, nickname, active, emailVerified,
                                                  verificationCode, createdAt, updatedAt);

    const auto domainRecipient = std::make_shared<domain::User>(userId2, email2, password, nickname, active,
                                                                emailVerified, verificationCode, createdAt, updatedAt);

    const auto id = faker::String::uuid();

    FriendInvitation persistenceFriendInvitation{id, sender, recipient, createdAt, updatedAt};

    EXPECT_CALL(*userMapper, mapToDomainUser(sender)).WillOnce(Return(domainSender));
    EXPECT_CALL(*userMapper, mapToDomainUser(recipient)).WillOnce(Return(domainRecipient));

    const auto domainFriendInvitation =
        channelInvitationMapper.mapToDomainFriendInvitation(persistenceFriendInvitation);

    ASSERT_EQ(domainFriendInvitation.getId(), id);
    ASSERT_EQ(domainFriendInvitation.getSender(), domainSender);
    ASSERT_EQ(domainFriendInvitation.getRecipient(), domainRecipient);
    ASSERT_EQ(domainFriendInvitation.getCreatedAt(), createdAt);
    ASSERT_EQ(domainFriendInvitation.getUpdatedAt(), updatedAt);
}
