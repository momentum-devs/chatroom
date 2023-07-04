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

TEST_F(UserChannelMapperTest, givenPersistenceUserChannel_shouldMapToDomainUserChannel)
{
    const auto userId = "userId";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto active = true;
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto user = std::make_shared<User>(userId, email, password, nickname, active, createdAt, updatedAt);

    const auto domainUser =
        std::make_shared<domain::User>(userId, email, password, nickname, active, createdAt, updatedAt);

    const auto channelId = "channelId";
    const auto name = "name";
    const auto creatorId = "creatorId";

    const auto channel = std::make_shared<Channel>(channelId, name, creatorId, createdAt, updatedAt);

    const auto domainChannel = std::make_shared<domain::Channel>(channelId, name, creatorId, createdAt, updatedAt);

    const auto id = "id";

    UserChannel persistenceUserChannel{id, user, channel, createdAt, updatedAt};

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    EXPECT_CALL(*channelMapper, mapToDomainChannel(channel)).WillOnce(Return(domainChannel));

    const auto domainUserChannel = userChannelMapper.mapToDomainUserChannel(persistenceUserChannel);

    ASSERT_EQ(domainUserChannel.getId(), id);
    ASSERT_EQ(domainUserChannel.getUser(), domainUser);
    ASSERT_EQ(domainUserChannel.getChannel(), domainChannel);
    ASSERT_EQ(domainUserChannel.getCreatedAt(), createdAt);
    ASSERT_EQ(domainUserChannel.getUpdatedAt(), updatedAt);
}
