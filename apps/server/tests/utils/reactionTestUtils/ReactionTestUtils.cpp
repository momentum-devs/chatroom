#include "ReactionTestUtils.h"

#include <format>
#include <utility>

#include "Reaction.odb.h"
#include "server/infrastructure/database/tables/ChannelTable.h"
#include "server/infrastructure/database/tables/ReactionTable.h"

namespace server::tests
{

ReactionTestUtils::ReactionTestUtils(std::shared_ptr<odb::pgsql::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      reactionTestFactory{std::make_unique<ReactionTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()},
      messageTestFactory{std::make_unique<MessageTestFactory>()},
      channelTestFactory{std::make_unique<ChannelTestFactory>()}
{
}

void ReactionTestUtils::persist(const std::shared_ptr<infrastructure::Reaction>& reaction)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(reaction);

    transaction.commit();
}

std::shared_ptr<infrastructure::Reaction>
ReactionTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& userInput,
                                    const std::shared_ptr<infrastructure::Message>& messageInput)
{
    const auto user = userInput ? userInput : userTestFactory->createPersistentUser();

    const auto message = messageInput ? messageInput :
                                        messageTestFactory->createPersistentMessage(
                                            user, channelTestFactory->createPersistentChannel(user), nullptr);

    auto reaction = reactionTestFactory->createPersistentReaction(user, message);

    persist(reaction);

    return reaction;
}

std::shared_ptr<infrastructure::Reaction> ReactionTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::Reaction> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Reaction> foundReaction(
        databaseClient->query_one<infrastructure::Reaction>(query::id == id));

    transaction.commit();

    return foundReaction;
}

std::shared_ptr<infrastructure::Reaction> ReactionTestUtils::find(const std::string& userId,
                                                                  const std::string& messageId)
{
    typedef odb::query<infrastructure::Reaction> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Reaction> foundReaction(databaseClient->query_one<infrastructure::Reaction>(
        query::user->id == userId && query::message->id == messageId));

    transaction.commit();

    return foundReaction;
}

void ReactionTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(std::format("DELETE FROM \"{}\";", infrastructure::reactionTableName));

    transaction.commit();
}
}
