#include "ConversationMapperImpl.h"

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

class ConversationMapperTest : public Test
{
public:
    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<ChannelMapperMock> channelMapper = std::make_shared<StrictMock<ChannelMapperMock>>();

    ConversationMapperImpl conversationMapper{userMapper, channelMapper};
};

TEST_F(ConversationMapperTest, givenPersistenceConversationWithUsers_shouldMapToDomainConversation)
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

    const auto recipient = std::make_shared<User>(userId2, email2, password, nickname, active, emailVerified,
                                                   verificationCode, createdAt, updatedAt);

    const auto domainRecipient = std::make_shared<domain::User>(userId2, email2, password, nickname, active,
                                                                 emailVerified, verificationCode, createdAt, updatedAt);

    const auto id = faker::String::uuid();

    Conversation persistenceConversation{id, user, recipient, nullptr, createdAt, updatedAt};

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    EXPECT_CALL(*userMapper, mapToDomainUser(recipient)).WillOnce(Return(domainRecipient));

    const auto domainConversation = conversationMapper.mapToDomainConversation(persistenceConversation);

    ASSERT_EQ(domainConversation.getId(), id);
    ASSERT_EQ(domainConversation.getUser(), domainUser);
    ASSERT_EQ(domainConversation.getRecipient(), domainRecipient);
    ASSERT_EQ(domainConversation.getChannel(), nullptr);
    ASSERT_EQ(domainConversation.getCreatedAt(), createdAt);
    ASSERT_EQ(domainConversation.getUpdatedAt(), updatedAt);
}


TEST_F(ConversationMapperTest, givenPersistenceConversationWithChannel_shouldMapToDomainConversation)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = std::make_shared<User>(userId, email, password, nickname, active, emailVerified,
                                             verificationCode, createdAt, updatedAt);

    const auto domainUser = std::make_shared<domain::User>(userId, email, password, nickname, active, emailVerified,
                                                           verificationCode, createdAt, updatedAt);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = std::make_shared<Channel>(channelId, name, user, createdAt, updatedAt);

    const auto domainChannel = std::make_shared<domain::Channel>(channelId, name, domainUser, createdAt, updatedAt);

    const auto id = faker::String::uuid();

    Conversation persistenceConversation{id, nullptr, nullptr, channel, createdAt, updatedAt};

    EXPECT_CALL(*channelMapper, mapToDomainChannel(channel)).WillOnce(Return(domainChannel));

    const auto domainConversation = conversationMapper.mapToDomainConversation(persistenceConversation);

    ASSERT_EQ(domainConversation.getId(), id);
    ASSERT_EQ(domainConversation.getUser(), nullptr);
    ASSERT_EQ(domainConversation.getRecipient(), nullptr);
    ASSERT_EQ(domainConversation.getChannel(), domainChannel);
    ASSERT_EQ(domainConversation.getCreatedAt(), createdAt);
    ASSERT_EQ(domainConversation.getUpdatedAt(), updatedAt);
}
