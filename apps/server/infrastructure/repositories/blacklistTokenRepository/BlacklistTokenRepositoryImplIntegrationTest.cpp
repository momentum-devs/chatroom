#include <odb/sqlite/database.hxx>

#include "gtest/gtest.h"

#include "BlacklistTokenRepositoryImpl.h"
#include "faker-cxx/Datatype.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/blacklistTokenMapper/BlacklistTokenMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/blacklistTokenTestUtils/BlacklistTokenTestUtils.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class BlacklistTokenRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        blacklistTokenTestUtils.truncateTable();
    }

    void TearDown() override
    {
        blacklistTokenTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    BlacklistTokenTestUtils blacklistTokenTestUtils{db};

    std::shared_ptr<server::infrastructure::BlacklistTokenMapper> blacklistTokenMapper =
        std::make_shared<server::infrastructure::BlacklistTokenMapperImpl>();

    std::shared_ptr<server::domain::BlacklistTokenRepository> blacklistTokenRepository =
        std::make_shared<server::infrastructure::BlacklistTokenRepositoryImpl>(db, blacklistTokenMapper);
};

TEST_F(BlacklistTokenRepositoryIntegrationTest, shouldCreateBlacklistToken)
{
    const auto id = faker::String::uuid();
    const auto token = faker::String::alphanumeric(32);

    const auto blacklistToken = blacklistTokenRepository->createBlacklistToken({id, token});

    ASSERT_EQ(blacklistToken->getToken(), token);
}

TEST_F(BlacklistTokenRepositoryIntegrationTest, shouldFindExistingBlacklistTokenByToken)
{
    const auto blacklistToken = blacklistTokenTestUtils.createAndPersist();

    const auto foundBlacklistToken = blacklistTokenRepository->findBlacklistTokenByToken({blacklistToken->getToken()});

    ASSERT_TRUE(foundBlacklistToken);
    ASSERT_EQ(foundBlacklistToken->get()->getToken(), blacklistToken->getToken());
}
