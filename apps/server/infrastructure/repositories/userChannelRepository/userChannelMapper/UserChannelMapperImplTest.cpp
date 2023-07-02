#include "UserChannelMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperMock.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class UserChannelMapperTest : public Test
{
public:
    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<ChannelMapperMock> channelMapper = std::make_shared<StrictMock<ChannelMapperMock>>();

    UserChannelMapperImpl userChannelMapper{userMapper, channelMapper};
};

TEST_F(UserChannelMapperTest, givenUserChannelModel_shouldMapToDomainUserChannel)
{
    const auto userId = "userId";
    const auto channelId = "channelId";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    UserChannel userChannelModel{id, email, password, nickname, createdAt, updatedAt};

    const auto domainUserChannel = userMapper.mapToDomainUser(userModel);

    ASSERT_EQ(domainUserChannel.getId(), id);
    ASSERT_EQ(domainUserChannel.getUser(), domainUser);
    ASSERT_EQ(domainUserChannel.getChannel(), domainChannel);
    ASSERT_EQ(domainUserChannel.getCreatedAt(), createdAt);
    ASSERT_EQ(domainUserChannel.getUpdatedAt(), updatedAt);
}
