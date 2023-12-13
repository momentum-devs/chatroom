#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>

#include "BlacklistToken.h"
#include "server/tests/factories/blacklistTokenTestFactory/BlacklistTokenTestFactory.h"

namespace server::tests
{
class BlacklistTokenTestUtils
{
public:
    explicit BlacklistTokenTestUtils(std::shared_ptr<odb::sqlite::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::BlacklistToken>& blacklistToken);
    std::shared_ptr<infrastructure::BlacklistToken> createAndPersist();
    std::shared_ptr<infrastructure::BlacklistToken> findByToken(const std::string& token);
    void truncateTable();

private:
    std::shared_ptr<odb::sqlite::database> databaseClient;
    std::unique_ptr<BlacklistTokenTestFactory> blacklistTokenTestFactory;
};
}
