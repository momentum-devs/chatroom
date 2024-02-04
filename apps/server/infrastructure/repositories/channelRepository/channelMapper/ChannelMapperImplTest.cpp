#include "ChannelMapperImpl.h"

#include "gtest/gtest.h"

#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperMock.h"

#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ChannelMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();

    ChannelMapperImpl channelMapper{userMapper};
};

TEST_F(ChannelMapperTest, givenPersistenceChannel_shouldMapToDomainChannel)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getResetPasswordCode().get(), user->getCreatedAt(),
        user->getUpdatedAt(),
        user->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : user->getAvatarUrl().get());

    const auto channel = channelTestFactory.createPersistentChannel(user);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    const auto domainChannel = channelMapper.mapToDomainChannel(channel);

    ASSERT_EQ(domainChannel->getId(), channel->getId());
    ASSERT_EQ(domainChannel->getName(), channel->getName());
    ASSERT_EQ(domainChannel->getCreator(), domainUser);
    ASSERT_EQ(domainChannel->getCreatedAt(), channel->getCreatedAt());
    ASSERT_EQ(domainChannel->getUpdatedAt(), channel->getUpdatedAt());
    ASSERT_EQ(domainChannel->getAvatarUrl(), channel->getAvatarUrl().get());
}

TEST_F(ChannelMapperTest, givenDomainChannel_shouldMapToPersistenceChannel)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getResetPasswordCode().get(), user->getCreatedAt(),
        user->getUpdatedAt(),
        user->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : user->getAvatarUrl().get());

    const auto domainChannel = channelTestFactory.createDomainChannel(domainUser);

    EXPECT_CALL(*userMapper, mapToPersistenceUser(domainUser)).WillOnce(Return(user));

    const auto persistenceChannel = channelMapper.mapToPersistenceChannel(domainChannel);

    ASSERT_EQ(persistenceChannel->getId(), domainChannel->getId());
    ASSERT_EQ(persistenceChannel->getName(), domainChannel->getName());
    ASSERT_EQ(persistenceChannel->getCreator(), user);
    ASSERT_EQ(persistenceChannel->getCreatedAt(), domainChannel->getCreatedAt());
    ASSERT_EQ(persistenceChannel->getUpdatedAt(), domainChannel->getUpdatedAt());
    ASSERT_EQ(persistenceChannel->getAvatarUrl().get(), domainChannel->getAvatarUrl());
}
