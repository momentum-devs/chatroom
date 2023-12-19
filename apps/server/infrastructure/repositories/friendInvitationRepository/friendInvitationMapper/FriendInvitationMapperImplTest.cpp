#include "FriendInvitationMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"

#include "server/tests/factories/friendInvitationTestFactory/FriendInvitationTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class FriendInvitationMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;

    FriendInvitationTestFactory friendInvitationTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();

    FriendInvitationMapperImpl channelInvitationMapper{userMapper};
};

TEST_F(FriendInvitationMapperTest, givenPersistenceFriendInvitation_shouldMapToDomainFriendInvitation)
{
    const auto sender = userTestFactory.createPersistentUser();

    const auto domainSender = std::make_shared<domain::User>(
        sender->getId(), sender->getEmail(), sender->getPassword(), sender->getNickname(), sender->isActive(),
        sender->isEmailVerified(), sender->getVerificationCode(), sender->getCreatedAt(), sender->getUpdatedAt(),
        sender->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : sender->getAvatarUrl().get());

    const auto recipient = userTestFactory.createPersistentUser();

    const auto domainRecipient = std::make_shared<domain::User>(
        sender->getId(), sender->getEmail(), sender->getPassword(), sender->getNickname(), sender->isActive(),
        sender->isEmailVerified(), sender->getVerificationCode(), sender->getCreatedAt(), sender->getUpdatedAt(),
        sender->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : sender->getAvatarUrl().get());

    const auto persistenceFriendInvitation =
        friendInvitationTestFactory.createPersistentFriendInvitation(sender, recipient);

    EXPECT_CALL(*userMapper, mapToDomainUser(sender)).WillOnce(Return(domainSender));
    EXPECT_CALL(*userMapper, mapToDomainUser(recipient)).WillOnce(Return(domainRecipient));

    const auto domainFriendInvitation =
        channelInvitationMapper.mapToDomainFriendInvitation(*persistenceFriendInvitation);

    ASSERT_EQ(domainFriendInvitation.getId(), persistenceFriendInvitation->getId());
    ASSERT_EQ(domainFriendInvitation.getSender(), domainSender);
    ASSERT_EQ(domainFriendInvitation.getRecipient(), domainRecipient);
    ASSERT_EQ(domainFriendInvitation.getCreatedAt(), persistenceFriendInvitation->getCreatedAt());
}
