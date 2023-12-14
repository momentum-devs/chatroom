#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "server/application/errors/InvalidTokenError.h"
#include "server/domain/repositories/blacklistTokenRepository/BlacklistTokenRepository.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/blacklistTokenMapper/BlacklistTokenMapperImpl.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/BlacklistTokenRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/blacklistTokenTestUtils/BlacklistTokenTestUtils.h"
#include "TokenServiceImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::tests;
using namespace server::application;
using namespace server::infrastructure;
using namespace server::domain;

namespace
{
const auto jwtSecret = "12321313423565365654546654121890008";
const auto jwtExpiresIn = 86400;
}

class TokenServiceImplIntegrationTest : public Test
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

    std::shared_ptr<BlacklistTokenMapper> blacklistTokenMapper = std::make_shared<BlacklistTokenMapperImpl>();

    std::shared_ptr<BlacklistTokenRepository> blacklistTokenRepository =
        std::make_shared<BlacklistTokenRepositoryImpl>(db, blacklistTokenMapper);

    TokenServiceImpl tokenService{jwtSecret, jwtExpiresIn, blacklistTokenRepository};
};

TEST_F(TokenServiceImplIntegrationTest, shouldCreateTokenAndHaveTheSameUserIdAfterVerifyingToken)
{
    const auto userId = faker::String::uuid();

    const auto token = tokenService.createToken(userId);

    const auto [actualUserId] = tokenService.verifyToken(token);

    ASSERT_EQ(actualUserId, userId);
}

TEST_F(TokenServiceImplIntegrationTest, shouldInvalidateToken)
{
    const auto userId = faker::String::uuid();

    const auto token = tokenService.createToken(userId);

    ASSERT_NO_THROW(tokenService.verifyToken(token));

    tokenService.invalidateToken(token);

    ASSERT_THROW(tokenService.verifyToken(token), errors::InvalidTokenError);

    const auto foundBlacklistToken = blacklistTokenTestUtils.findByToken(token);

    ASSERT_TRUE(foundBlacklistToken);
}
