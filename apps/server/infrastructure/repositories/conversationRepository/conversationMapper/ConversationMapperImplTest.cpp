#include "ConversationMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperMock.h"

#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/conversationTestFactory/ConversationTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ConversationMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;
    ConversationTestFactory conversationTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<ChannelMapperMock> channelMapper = std::make_shared<StrictMock<ChannelMapperMock>>();

    ConversationMapperImpl conversationMapper{userMapper, channelMapper};
};

TEST_F(ConversationMapperTest, givenPersistenceConversationWithUsers_shouldMapToDomainConversation)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getCreatedAt(), user->getUpdatedAt());

    const auto recipient = userTestFactory.createPersistentUser();

    const auto domainRecipient = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getCreatedAt(), user->getUpdatedAt());

    const auto conversation = conversationTestFactory.createPersistentUsersConversation(user, recipient);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    EXPECT_CALL(*userMapper, mapToDomainUser(recipient)).WillOnce(Return(domainRecipient));

    const auto domainConversation = conversationMapper.mapToDomainConversation(*conversation);

    ASSERT_EQ(domainConversation.getId(), conversation->getId());
    ASSERT_EQ(domainConversation.getUser(), domainUser);
    ASSERT_EQ(domainConversation.getRecipient(), domainRecipient);
    ASSERT_EQ(domainConversation.getChannel(), nullptr);
    ASSERT_EQ(domainConversation.getCreatedAt(), conversation->getCreatedAt());
    ASSERT_EQ(domainConversation.getUpdatedAt(), conversation->getUpdatedAt());
}

TEST_F(ConversationMapperTest, givenPersistenceConversationWithChannel_shouldMapToDomainConversation)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getCreatedAt(), user->getUpdatedAt());

    const auto channel = channelTestFactory.createPersistentChannel(user);

    const auto domainChannel = channelMapper->mapToDomainChannel(channel);

    const auto conversation = conversationTestFactory.createPersistentChannelConversation(channel);

    EXPECT_CALL(*channelMapper, mapToDomainChannel(channel)).WillOnce(Return(domainChannel));

    const auto domainConversation = conversationMapper.mapToDomainConversation(*conversation);

    ASSERT_EQ(domainConversation.getId(), conversation->getId());
    ASSERT_EQ(domainConversation.getUser(), nullptr);
    ASSERT_EQ(domainConversation.getRecipient(), nullptr);
    ASSERT_EQ(domainConversation.getChannel(), domainChannel);
    ASSERT_EQ(domainConversation.getCreatedAt(), conversation->getCreatedAt());
    ASSERT_EQ(domainConversation.getUpdatedAt(), conversation->getUpdatedAt());
}
