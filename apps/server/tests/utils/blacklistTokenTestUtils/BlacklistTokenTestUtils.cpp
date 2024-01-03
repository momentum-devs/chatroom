#include "BlacklistTokenTestUtils.h"

#include <utility>

#include "BlacklistToken.odb.h"
#include <fmt/format.h>
#include "server/infrastructure/database/tables/BlacklistTokenTable.h"

namespace server::tests
{

BlacklistTokenTestUtils::BlacklistTokenTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      blacklistTokenTestFactory{std::make_unique<BlacklistTokenTestFactory>()}
{
}

void BlacklistTokenTestUtils::persist(const std::shared_ptr<infrastructure::BlacklistToken>& blacklistToken)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(blacklistToken);

    transaction.commit();
}

std::shared_ptr<infrastructure::BlacklistToken> BlacklistTokenTestUtils::createAndPersist()
{
    auto blacklistToken = blacklistTokenTestFactory->createPersistentBlacklistToken();

    persist(blacklistToken);

    return blacklistToken;
}

std::shared_ptr<infrastructure::BlacklistToken> BlacklistTokenTestUtils::findByToken(const std::string& token)
{
    typedef odb::query<infrastructure::BlacklistToken> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::BlacklistToken> foundBlacklistToken(
        databaseClient->query_one<infrastructure::BlacklistToken>(query::token == token));

    transaction.commit();

    return foundBlacklistToken;
}

void BlacklistTokenTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(fmt::format("DELETE FROM \"{}\";", infrastructure::blacklistTokenTableName));

    transaction.commit();
}
}
