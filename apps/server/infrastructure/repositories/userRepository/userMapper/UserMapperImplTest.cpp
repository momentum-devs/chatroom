#include "UserMapperImpl.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class UserMapperTest : public Test
{
public:
    UserMapperImpl userMapper;
};

TEST_F(UserMapperTest, givenUserModel_shouldMapToDomainUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    Models::User userModel;
    userModel.setAttribute("email", email);
    userModel.setAttribute("password", password);
    userModel.setAttribute("nickname", nickname);

    const auto domainUser = userMapper.mapToDomainUser(userModel);

    ASSERT_EQ(domainUser.getId(), 0);
    ASSERT_EQ(domainUser.getEmail(), email);
    ASSERT_EQ(domainUser.getPassword(), password);
    ASSERT_EQ(domainUser.getNickname(), nickname);
}
