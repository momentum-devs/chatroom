#include "UserChannelTestUtils.h"

#include <format>
#include <utility>

#include "server/infrastructure/database/tables/ChannelTable.h"
#include "server/infrastructure/database/tables/UserChannelTable.h"
#include "UserChannel.odb.h"

namespace server::tests
{

UserChannelTestUtils::UserChannelTestUtils(std::shared_ptr<odb::pgsql::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      userChannelTestFactory{std::make_unique<UserChannelTestFactory>()},
      channelTestFactory{std::make_unique<ChannelTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()}
{
}

void UserChannelTestUtils::persist(const std::shared_ptr<infrastructure::UserChannel>& userChannel)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(userChannel);

    transaction.commit();
}

std::shared_ptr<infrastructure::UserChannel>
UserChannelTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& userInput,
                                       const std::shared_ptr<infrastructure::Channel>& channelInput)
{
    const auto user = userInput ? userInput : userTestFactory->createPersistentUser();

    const auto channel = channelInput ? channelInput : channelTestFactory->createPersistentChannel(user);

    auto userChannel = userChannelTestFactory->createPersistentUserChannel(user, channel);

    persist(userChannel);

    return userChannel;
}

std::shared_ptr<infrastructure::UserChannel> UserChannelTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::UserChannel> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::UserChannel> foundUserChannel(
        databaseClient->query_one<infrastructure::UserChannel>(query::id == id));

    transaction.commit();

    return foundUserChannel;
}

void UserChannelTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(std::format("DELETE FROM \"{}\";", infrastructure::userChannelTableName));

    transaction.commit();
}
}
