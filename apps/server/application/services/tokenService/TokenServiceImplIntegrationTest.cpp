#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "server/application/errors/InvalidTokenError.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "TokenServiceImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::tests;
using namespace server::application;

namespace
{
const auto jwtSecret = "12321313423565365654546654121890008";
const auto jwtExpiresIn = 86400;
}

class TokenServiceImplIntegrationTest : public Test
{
public:
    TokenServiceImpl tokenService{jwtSecret, jwtExpiresIn};
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
}
