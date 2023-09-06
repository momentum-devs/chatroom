#include "ChannelInvitationMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ChannelInvitationMapperTest : public Test
{
public:
    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<ChannelMapperMock> channelMapper = std::make_shared<StrictMock<ChannelMapperMock>>();

    ChannelInvitationMapperImpl channelInvitationMapper{userMapper, channelMapper};
};

TEST_F(ChannelInvitationMapperTest, givenPersistenceChannelInvitation_shouldMapToDomainChannelInvitation)
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

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();
    const auto creatorId = sender->getId();

    const auto channel = std::make_shared<Channel>(channelId, name, creatorId, createdAt, updatedAt);

    const auto domainChannel = std::make_shared<domain::Channel>(channelId, name, creatorId, createdAt, updatedAt);

    const auto id = faker::String::uuid();

    ChannelInvitation persistenceChannelInvitation{id, sender, recipient, channel, createdAt, updatedAt};

    EXPECT_CALL(*userMapper, mapToDomainUser(sender)).WillOnce(Return(domainSender));
    EXPECT_CALL(*userMapper, mapToDomainUser(recipient)).WillOnce(Return(domainRecipient));

    EXPECT_CALL(*channelMapper, mapToDomainChannel(channel)).WillOnce(Return(domainChannel));

    const auto domainChannelInvitation =
        channelInvitationMapper.mapToDomainChannelInvitation(persistenceChannelInvitation);

    ASSERT_EQ(domainChannelInvitation.getId(), id);
    ASSERT_EQ(domainChannelInvitation.getSender(), domainSender);
    ASSERT_EQ(domainChannelInvitation.getRecipient(), domainRecipient);
    ASSERT_EQ(domainChannelInvitation.getChannel(), domainChannel);
    ASSERT_EQ(domainChannelInvitation.getCreatedAt(), createdAt);
    ASSERT_EQ(domainChannelInvitation.getUpdatedAt(), updatedAt);
}
