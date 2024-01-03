#include "ChannelTestUtils.h"

#include <utility>

#include "Channel.odb.h"
#include "fmt/format.h"
#include "server/infrastructure/database/tables/ChannelTable.h"

namespace server::tests
{

ChannelTestUtils::ChannelTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      channelTestFactory{std::make_unique<ChannelTestFactory>()},
      userTestUtils{std::make_unique<UserTestUtils>(databaseClient)}
{
}

void ChannelTestUtils::persist(const std::shared_ptr<infrastructure::Channel>& channel)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(channel);

    transaction.commit();
}

std::shared_ptr<infrastructure::Channel>
ChannelTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& user)
{
    const auto creator = user ? user : userTestUtils->createAndPersist();

    auto channel = channelTestFactory->createPersistentChannel(creator);

    persist(channel);

    return channel;
}

std::shared_ptr<infrastructure::Channel> ChannelTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::Channel> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Channel> foundChannel(
        databaseClient->query_one<infrastructure::Channel>(query::id == id));

    transaction.commit();

    return foundChannel;
}

void ChannelTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(fmt::format("DELETE FROM \"{}\";", infrastructure::channelTableName));

    transaction.commit();
}
}
