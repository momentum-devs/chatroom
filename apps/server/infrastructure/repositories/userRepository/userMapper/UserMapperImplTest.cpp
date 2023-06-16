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
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    User userModel{id, email, password, nickname, createdAt, updatedAt};

    const auto domainUser = userMapper.mapToDomainUser(userModel);

    ASSERT_EQ(domainUser.getId(), id);
    ASSERT_EQ(domainUser.getEmail(), email);
    ASSERT_EQ(domainUser.getPassword(), password);
    ASSERT_EQ(domainUser.getNickname(), nickname);
    ASSERT_EQ(domainUser.getCreatedAt(), "2023-06-16");
    ASSERT_EQ(domainUser.getUpdatedAt(), "2023-06-16");
}
