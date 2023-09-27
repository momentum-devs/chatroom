#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.h"
#include "Conversation.h"
#include "Conversation.odb.h"
#include "ConversationRepositoryImpl.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ConversationRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"conversations\";");

        db->execute("DELETE FROM \"channels\";");

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"conversations\";");

        db->execute("DELETE FROM \"channels\";");

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    std::shared_ptr<User> createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto user = std::make_shared<User>(id, email, password, email, false, false, "123", currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    std::shared_ptr<Channel> createChannel(const std::string& id, const std::string& name, const std::string& creatorId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto channel = std::make_shared<Channel>(id, name, creatorId, currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();

        return channel;
    }

    Conversation createUsersConversation(const std::string& id, std::shared_ptr<User> user,
                                         std::shared_ptr<User> recipient)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        Conversation conversation{id, std::move(user), std::move(recipient), nullptr, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(conversation);

        transaction.commit();

        return conversation;
    }

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>();

    std::shared_ptr<ConversationMapper> conversationMapper =
        std::make_shared<ConversationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::ConversationRepository> conversationRepository =
        std::make_shared<server::infrastructure::ConversationRepositoryImpl>(db, conversationMapper, userMapper,
                                                                             channelMapper);
};

TEST_F(ConversationRepositoryIntegrationTest, shouldCreateUsersConversation)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto conversation = conversationRepository->createConversation(
        {id, userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(recipient), std::nullopt});

    ASSERT_EQ(conversation.getUser()->getId(), userId);
    ASSERT_EQ(conversation.getRecipient()->getId(), recipientId);
    ASSERT_EQ(conversation.getChannel(), nullptr);
}

TEST_F(ConversationRepositoryIntegrationTest, shouldCreateChannelConversation)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, userId);

    const auto conversation = conversationRepository->createConversation(
        {id, std::nullopt, std::nullopt, channelMapper->mapToDomainChannel(channel)});

    ASSERT_EQ(conversation.getChannel()->getId(), channelId);
    ASSERT_EQ(conversation.getUser(), nullptr);
    ASSERT_EQ(conversation.getRecipient(), nullptr);
}

TEST_F(ConversationRepositoryIntegrationTest, shouldDeleteExistingConversation)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto conversation = createUsersConversation(id, user, recipient);

    const auto domainConversation = domain::Conversation{
        conversation.getId(),        userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(recipient), nullptr,
        conversation.getCreatedAt(), conversation.getUpdatedAt()};

    conversationRepository->deleteConversation({domainConversation});

    typedef odb::query<Conversation> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<Conversation> foundConversation(db->query_one<Conversation>(query::id == id));

        ASSERT_FALSE(foundConversation);

        transaction.commit();
    }
}

TEST_F(ConversationRepositoryIntegrationTest, delete_givenNonExistingConversation_shouldThrowError)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::recentDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto userId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto domainConversation = domain::Conversation{
        id, userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(recipient), nullptr, createdAt, updatedAt};

    ASSERT_ANY_THROW(conversationRepository->deleteConversation({domainConversation}));
}

TEST_F(ConversationRepositoryIntegrationTest, shouldFindConversationByUserIds)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto conversation = conversationRepository->createConversation(
        {id, userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(recipient), std::nullopt});

    const auto foundConversation = conversationRepository->findConversationByUserAndRecipientIds({userId, recipientId});

    ASSERT_TRUE(foundConversation);
    ASSERT_EQ(foundConversation->getId(), conversation.getId());
}

TEST_F(ConversationRepositoryIntegrationTest, shouldFindConversationByChannelId)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, userId);

    const auto conversation = conversationRepository->createConversation(
        {id, std::nullopt, std::nullopt, channelMapper->mapToDomainChannel(channel)});

    const auto foundConversation = conversationRepository->findConversationByChannelId({channelId});

    ASSERT_TRUE(foundConversation);
    ASSERT_EQ(foundConversation->getId(), conversation.getId());
}

TEST_F(ConversationRepositoryIntegrationTest, shouldFindConversationById)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, userId);

    const auto conversation = conversationRepository->createConversation(
        {id, std::nullopt, std::nullopt, channelMapper->mapToDomainChannel(channel)});

    const auto foundConversation = conversationRepository->findConversationById({id});

    ASSERT_TRUE(foundConversation);
    ASSERT_EQ(foundConversation->getId(), conversation.getId());
}
