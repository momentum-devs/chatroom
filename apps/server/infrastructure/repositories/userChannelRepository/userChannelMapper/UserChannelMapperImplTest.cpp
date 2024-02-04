#include "UserChannelMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperMock.h"

#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/userChannelTestFactory/UserChannelTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class UserChannelMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;
    UserChannelTestFactory userChannelTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<ChannelMapperMock> channelMapper = std::make_shared<StrictMock<ChannelMapperMock>>();

    UserChannelMapperImpl userChannelMapper{userMapper, channelMapper};
};

TEST_F(UserChannelMapperTest, givenPersistenceUserChannel_shouldMapToDomainUserChannel)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getResetPasswordCode().get(), user->getCreatedAt(),
        user->getUpdatedAt(),
        user->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : user->getAvatarUrl().get());

    const auto channel = channelTestFactory.createPersistentChannel(user);

    const auto domainChannel = std::make_shared<domain::Channel>(
        channel->getId(), channel->getName(), domainUser, channel->getCreatedAt(), channel->getUpdatedAt(),
        channel->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : channel->getAvatarUrl().get());

    const auto userChannel = userChannelTestFactory.createPersistentUserChannel(user, channel);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    EXPECT_CALL(*channelMapper, mapToDomainChannel(channel)).WillOnce(Return(domainChannel));

    const auto domainUserChannel = userChannelMapper.mapToDomainUserChannel(*userChannel);

    ASSERT_EQ(domainUserChannel.getId(), userChannel->getId());
    ASSERT_EQ(domainUserChannel.getLastReadMessageId(), userChannel->getLastReadMessageId().get());
    ASSERT_EQ(domainUserChannel.getUser(), domainUser);
    ASSERT_EQ(domainUserChannel.getChannel(), domainChannel);
    ASSERT_EQ(domainUserChannel.getCreatedAt(), userChannel->getCreatedAt());
}
