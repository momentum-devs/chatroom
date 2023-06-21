#include "ChannelMapperImpl.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ChannelMapperTest : public Test
{
public:
    ChannelMapperImpl userMapper;
};

TEST_F(ChannelMapperTest, givenChannelModel_shouldMapToDomainChannel)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel userModel{id, email, password, nickname, createdAt, updatedAt};

    const auto domainChannel = userMapper.mapToDomainChannel(userModel);

    ASSERT_EQ(domainChannel.getId(), id);
    ASSERT_EQ(domainChannel.getEmail(), email);
    ASSERT_EQ(domainChannel.getPassword(), password);
    ASSERT_EQ(domainChannel.getNickname(), nickname);
    ASSERT_EQ(domainChannel.getCreatedAt(), "2023-06-16");
    ASSERT_EQ(domainChannel.getUpdatedAt(), "2023-06-16");
}
