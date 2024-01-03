#include "MessageTestUtils.h"

#include <utility>

#include <fmt/format.h>
#include "Message.odb.h"
#include "server/infrastructure/database/tables/MessageTable.h"

namespace server::tests
{

MessageTestUtils::MessageTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      messageTestFactory{std::make_unique<MessageTestFactory>()},
      channelTestFactory{std::make_unique<ChannelTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()},
      groupTestFactory{std::make_unique<GroupTestFactory>()}
{
}

void MessageTestUtils::persist(const std::shared_ptr<infrastructure::Message>& message)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(message);

    transaction.commit();
}

std::shared_ptr<infrastructure::Message>
MessageTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& senderInput,
                                   const std::shared_ptr<infrastructure::Channel>& channelInput)
{
    const auto sender = senderInput ? senderInput : userTestFactory->createPersistentUser();

    const auto channel = channelInput ? channelInput : channelTestFactory->createPersistentChannel(sender);

    auto message = messageTestFactory->createPersistentMessage(sender, channel, nullptr);

    persist(message);

    return message;
}

std::shared_ptr<infrastructure::Message>
MessageTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& senderInput,
                                   const std::shared_ptr<infrastructure::Group>& groupInput)
{
    const auto sender = senderInput ? senderInput : userTestFactory->createPersistentUser();

    const auto group = groupInput ? groupInput : groupTestFactory->createPersistentGroup();

    auto message = messageTestFactory->createPersistentMessage(sender, nullptr, group);

    persist(message);

    return message;
}

std::shared_ptr<infrastructure::Message> MessageTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::Message> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Message> foundMessage(
        databaseClient->query_one<infrastructure::Message>(query::id == id));

    transaction.commit();

    return foundMessage;
}

std::shared_ptr<infrastructure::Message> MessageTestUtils::findBySenderAndChannel(const std::string& senderId,
                                                                                  const std::string& channelId)
{
    typedef odb::query<infrastructure::Message> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Message> foundMessage(databaseClient->query_one<infrastructure::Message>(
        query::sender->id == senderId && query::channel->id == channelId));

    transaction.commit();

    return foundMessage;
}

std::shared_ptr<infrastructure::Message> MessageTestUtils::findBySenderAndGroup(const std::string& senderId,
                                                                                const std::string& groupId)
{
    typedef odb::query<infrastructure::Message> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Message> foundMessage(databaseClient->query_one<infrastructure::Message>(
        query::sender->id == senderId && query::group->id == groupId));

    transaction.commit();

    return foundMessage;
}

void MessageTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(fmt::format("DELETE FROM \"{}\";", infrastructure::messageTableName));

    transaction.commit();
}

}
