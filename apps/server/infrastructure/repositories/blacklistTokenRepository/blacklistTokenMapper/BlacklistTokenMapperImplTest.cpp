#include "BlacklistTokenMapperImpl.h"

#include "gtest/gtest.h"

#include "../../../../tests/factories/blacklistTokenTestFactory/BlacklistTokenTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class BlacklistTokenMapperTest : public Test
{
public:
    BlacklistTokenTestFactory blacklistTokenTestFactory;

    BlacklistTokenMapperImpl blacklistTokenMapper;
};

TEST_F(BlacklistTokenMapperTest, givenPersistenceBlacklistToken_shouldMapToDomainBlacklistToken)
{
    const auto persistenceBlacklistToken = blacklistTokenTestFactory.createPersistentBlacklistToken();

    const auto domainBlacklistToken = blacklistTokenMapper.mapToDomainBlacklistToken(persistenceBlacklistToken);

    ASSERT_EQ(domainBlacklistToken->getId(), persistenceBlacklistToken->getId());
    ASSERT_EQ(domainBlacklistToken->getToken(), persistenceBlacklistToken->getToken());
    ASSERT_EQ(domainBlacklistToken->getExpiresAt(), persistenceBlacklistToken->getExpiresAt());
}
