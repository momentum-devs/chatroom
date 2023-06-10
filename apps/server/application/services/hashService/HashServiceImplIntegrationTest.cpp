#include "gtest/gtest.h"

#include "server/infrastructure/database/management/DatabaseManagerFactory.h"
#include "server/infrastructure/errors/UserRepositoryError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "TokenServiceImpl.h"

using namespace ::testing;
using namespace server;
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
    const auto userId = 1;

    const auto token = tokenService.createToken(userId);

    const auto userIdFromToken = tokenService.getUserIdFromToken(token);

    ASSERT_EQ(userIdFromToken, userId);
}
