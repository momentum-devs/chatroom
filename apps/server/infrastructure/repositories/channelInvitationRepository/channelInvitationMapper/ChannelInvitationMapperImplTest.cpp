#include "ChannelInvitationMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperMock.h"

#include "server/tests/factories/channelInvitationTestFactory/ChannelInvitationTestFactory.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ChannelInvitationMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;
    ChannelInvitationTestFactory channelInvitationTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<ChannelMapperMock> channelMapper = std::make_shared<StrictMock<ChannelMapperMock>>();

    ChannelInvitationMapperImpl channelInvitationMapper{userMapper, channelMapper};
};

TEST_F(ChannelInvitationMapperTest, givenPersistenceChannelInvitation_shouldMapToDomainChannelInvitation)
{
    const auto sender = userTestFactory.createPersistentUser();

    const auto domainSender = std::make_shared<domain::User>(
        sender->getId(), sender->getEmail(), sender->getPassword(), sender->getNickname(), sender->isActive(),
        sender->isEmailVerified(), sender->getVerificationCode(), sender->getCreatedAt(), sender->getUpdatedAt());

    const auto recipient = userTestFactory.createPersistentUser();

    const auto domainRecipient = std::make_shared<domain::User>(
        sender->getId(), sender->getEmail(), sender->getPassword(), sender->getNickname(), sender->isActive(),
        sender->isEmailVerified(), sender->getVerificationCode(), sender->getCreatedAt(), sender->getUpdatedAt());

    const auto channel = channelTestFactory.createPersistentChannel(sender);

    const auto domainChannel = std::make_shared<domain::Channel>(channel->getId(), channel->getName(), domainSender,
                                                                 channel->getCreatedAt(), channel->getUpdatedAt());

    const auto persistenceChannelInvitation =
        channelInvitationTestFactory.createPersistentChannelInvitation(sender, recipient, channel);

    EXPECT_CALL(*userMapper, mapToDomainUser(sender)).WillOnce(Return(domainSender));
    EXPECT_CALL(*userMapper, mapToDomainUser(recipient)).WillOnce(Return(domainRecipient));

    EXPECT_CALL(*channelMapper, mapToDomainChannel(channel)).WillOnce(Return(domainChannel));

    const auto domainChannelInvitation =
        channelInvitationMapper.mapToDomainChannelInvitation(*persistenceChannelInvitation);

    ASSERT_EQ(domainChannelInvitation.getId(), persistenceChannelInvitation->getId());
    ASSERT_EQ(domainChannelInvitation.getSender(), domainSender);
    ASSERT_EQ(domainChannelInvitation.getRecipient(), domainRecipient);
    ASSERT_EQ(domainChannelInvitation.getChannel(), domainChannel);
    ASSERT_EQ(domainChannelInvitation.getCreatedAt(), persistenceChannelInvitation->getCreatedAt());
}
