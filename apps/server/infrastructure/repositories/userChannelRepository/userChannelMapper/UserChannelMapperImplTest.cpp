#include "UserChannelMapperImpl.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class UserChannelMapperTest : public Test
{
public:
    UserChannelMapperImpl userChannelMapper;
};

TEST_F(UserChannelMapperTest, givenUserChannelModel_shouldMapToDomainUserChannel)
{
    const auto id = "id";
    const auto userId = "userId";
    const auto channelId = "channelId";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    UserChannel userChannelModel{id, email, password, nickname, createdAt, updatedAt};

    const auto domainUserChannel = userMapper.mapToDomainUser(userModel);

    ASSERT_EQ(domainUserChannel.getId(), id);
    ASSERT_EQ(domainUserChannel.getUserId(), userId);
    ASSERT_EQ(domainUserChannel.getChannelId(), channelId);
    ASSERT_EQ(domainUserChannel.getCreatedAt(), createdAt);
    ASSERT_EQ(domainUserChannel.getUpdatedAt(), updatedAt);
}
