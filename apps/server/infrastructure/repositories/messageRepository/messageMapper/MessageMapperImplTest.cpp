#include "MessageMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperMock.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperMock.h"

#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class MessageMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;
    MessageTestFactory messageTestFactory;
    GroupTestFactory groupTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<ChannelMapperMock> channelMapper = std::make_shared<StrictMock<ChannelMapperMock>>();
    std::shared_ptr<GroupMapperMock> groupMapper = std::make_shared<StrictMock<GroupMapperMock>>();

    MessageMapperImpl messageMapper{userMapper, channelMapper, groupMapper};
};

TEST_F(MessageMapperTest, givenPersistenceMessageWithChannel_shouldMapToDomainMessage)
{
    const auto sender = userTestFactory.createPersistentUser();

    const auto domainSender = std::make_shared<domain::User>(
        sender->getId(), sender->getEmail(), sender->getPassword(), sender->getNickname(), sender->isActive(),
        sender->isEmailVerified(), sender->getVerificationCode(), sender->getCreatedAt(), sender->getUpdatedAt());

    const auto channel = channelTestFactory.createPersistentChannel(sender);

    const auto domainChannel = std::make_shared<domain::Channel>(channel->getId(), channel->getName(), domainSender,
                                                                 channel->getCreatedAt(), channel->getUpdatedAt());

    const auto persistenceMessage = messageTestFactory.createPersistentMessage(sender, channel, nullptr);

    EXPECT_CALL(*userMapper, mapToDomainUser(sender)).WillOnce(Return(domainSender));

    EXPECT_CALL(*channelMapper, mapToDomainChannel(channel)).WillOnce(Return(domainChannel));

    const auto domainMessage = messageMapper.mapToDomainMessage(persistenceMessage);

    ASSERT_EQ(domainMessage->getId(), persistenceMessage->getId());
    ASSERT_EQ(domainMessage->getContent(), persistenceMessage->getContent());
    ASSERT_EQ(domainMessage->getSender(), domainSender);
    ASSERT_EQ(domainMessage->getChannel(), domainChannel);
    ASSERT_EQ(domainMessage->getGroup(), nullptr);
    ASSERT_EQ(domainMessage->getCreatedAt(), persistenceMessage->getCreatedAt());
    ASSERT_EQ(domainMessage->getUpdatedAt(), persistenceMessage->getUpdatedAt());
}

TEST_F(MessageMapperTest, givenPersistenceMessageWithGroup_shouldMapToDomainMessage)
{
    const auto sender = userTestFactory.createPersistentUser();

    const auto domainSender = std::make_shared<domain::User>(
        sender->getId(), sender->getEmail(), sender->getPassword(), sender->getNickname(), sender->isActive(),
        sender->isEmailVerified(), sender->getVerificationCode(), sender->getCreatedAt(), sender->getUpdatedAt());

    const auto group = groupTestFactory.createPersistentGroup();

    const auto domainGroup = std::make_shared<domain::Group>(group->getId(), group->getCreatedAt());

    const auto persistenceMessage = messageTestFactory.createPersistentMessage(sender, nullptr, group);

    EXPECT_CALL(*userMapper, mapToDomainUser(sender)).WillOnce(Return(domainSender));

    EXPECT_CALL(*groupMapper, mapToDomainGroup(group)).WillOnce(Return(domainGroup));

    const auto domainMessage = messageMapper.mapToDomainMessage(persistenceMessage);

    ASSERT_EQ(domainMessage->getId(), persistenceMessage->getId());
    ASSERT_EQ(domainMessage->getContent(), persistenceMessage->getContent());
    ASSERT_EQ(domainMessage->getSender(), domainSender);
    ASSERT_EQ(domainMessage->getChannel(), nullptr);
    ASSERT_EQ(domainMessage->getGroup(), domainGroup);
    ASSERT_EQ(domainMessage->getCreatedAt(), persistenceMessage->getCreatedAt());
    ASSERT_EQ(domainMessage->getUpdatedAt(), persistenceMessage->getUpdatedAt());
}

TEST_F(MessageMapperTest, givenDomainMessageWithChannel_shouldMapToPersistenceMessage)
{
    const auto sender = userTestFactory.createPersistentUser();

    const auto domainSender = std::make_shared<domain::User>(
        sender->getId(), sender->getEmail(), sender->getPassword(), sender->getNickname(), sender->isActive(),
        sender->isEmailVerified(), sender->getVerificationCode(), sender->getCreatedAt(), sender->getUpdatedAt());

    const auto channel = channelTestFactory.createPersistentChannel(sender);

    const auto domainChannel = std::make_shared<domain::Channel>(channel->getId(), channel->getName(), domainSender,
                                                                 channel->getCreatedAt(), channel->getUpdatedAt());

    const auto domainMessage = messageTestFactory.createDomainMessage(domainSender, domainChannel, nullptr);

    EXPECT_CALL(*userMapper, mapToPersistenceUser(domainSender)).WillOnce(Return(sender));

    EXPECT_CALL(*channelMapper, mapToPersistenceChannel(domainChannel)).WillOnce(Return(channel));

    const auto persistenceMessage = messageMapper.mapToPersistenceMessage(domainMessage);

    ASSERT_EQ(persistenceMessage->getId(), domainMessage->getId());
    ASSERT_EQ(persistenceMessage->getContent(), domainMessage->getContent());
    ASSERT_EQ(persistenceMessage->getSender()->getId(), domainMessage->getSender()->getId());
    ASSERT_EQ(persistenceMessage->getChannel()->getId(), domainMessage->getChannel()->getId());
    ASSERT_EQ(persistenceMessage->getGroup(), nullptr);
    ASSERT_EQ(persistenceMessage->getCreatedAt(), domainMessage->getCreatedAt());
    ASSERT_EQ(persistenceMessage->getUpdatedAt(), domainMessage->getUpdatedAt());
}
