#include "ConversationTestUtils.h"

#include <format>
#include <utility>

#include "Conversation.odb.h"
#include "server/infrastructure/database/tables/ChannelTable.h"
#include "server/infrastructure/database/tables/ConversationTable.h"

namespace server::tests
{

ConversationTestUtils::ConversationTestUtils(std::shared_ptr<odb::pgsql::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      conversationTestFactory{std::make_unique<ConversationTestFactory>()},
      channelTestFactory{std::make_unique<ChannelTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()}
{
}

void ConversationTestUtils::persist(const std::shared_ptr<infrastructure::Conversation>& conversation)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(conversation);

    transaction.commit();
}

std::shared_ptr<infrastructure::Conversation>
ConversationTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& userInput,
                                        const std::shared_ptr<infrastructure::User>& recipientInput)
{
    const auto user = userInput ? userInput : userTestFactory->createPersistentUser();

    const auto recipient = recipientInput ? recipientInput : userTestFactory->createPersistentUser();

    auto conversation = conversationTestFactory->createPersistentUsersConversation(user, recipient);

    persist(conversation);

    return conversation;
}

std::shared_ptr<infrastructure::Conversation>
ConversationTestUtils::createAndPersist(const std::shared_ptr<infrastructure::Channel>& channelInput)
{
    const auto channelCreator = userTestFactory->createPersistentUser();

    const auto channel = channelInput ? channelInput : channelTestFactory->createPersistentChannel(channelCreator);

    auto conversation = conversationTestFactory->createPersistentChannelConversation(channel);

    persist(conversation);

    return conversation;
}

std::shared_ptr<infrastructure::Conversation> ConversationTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::Conversation> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Conversation> foundConversation(
        databaseClient->query_one<infrastructure::Conversation>(query::id == id));

    transaction.commit();

    return foundConversation;
}

std::shared_ptr<infrastructure::Conversation> ConversationTestUtils::findByUsers(const std::string& userId,
                                                                                 const std::string& recipientId)
{
    typedef odb::query<infrastructure::Conversation> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Conversation> foundConversation(
        databaseClient->query_one<infrastructure::Conversation>(query::user->id == userId &&
                                                                query::recipient->id == recipientId));

    transaction.commit();

    return foundConversation;
}

std::shared_ptr<infrastructure::Conversation> ConversationTestUtils::findByChannelId(const std::string& channelId)
{
    typedef odb::query<infrastructure::Conversation> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Conversation> foundConversation(
        databaseClient->query_one<infrastructure::Conversation>(query::channel->id == channelId));

    transaction.commit();

    return foundConversation;
}

void ConversationTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(std::format("DELETE FROM \"{}\";", infrastructure::conversationTableName));

    transaction.commit();
}

}
