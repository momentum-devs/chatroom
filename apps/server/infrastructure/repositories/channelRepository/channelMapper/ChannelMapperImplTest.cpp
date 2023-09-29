#include "ChannelMapperImpl.h"

#include "gtest/gtest.h"

#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperMock.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ChannelMapperTest : public Test
{
public:
    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();

    ChannelMapperImpl channelMapper{userMapper};
};

TEST_F(ChannelMapperTest, givenPersistenceChannel_shouldMapToDomainChannel)
{
    const auto userId = "userId";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto active = true;
    const auto emailVerified = false;
    const auto verificationCode = "verificationCode";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto user = std::make_shared<User>(userId, email, password, nickname, active, emailVerified, verificationCode,
                                             createdAt, updatedAt);

    const auto domainUser = std::make_shared<domain::User>(userId, email, password, nickname, active, emailVerified,
                                                           verificationCode, createdAt, updatedAt);

    const auto id = "id";
    const auto name = "name";

    const auto persistenceChannel = std::make_shared<Channel>(id, name, user, createdAt, updatedAt);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    const auto domainChannel = channelMapper.mapToDomainChannel(persistenceChannel);

    ASSERT_EQ(domainChannel->getId(), id);
    ASSERT_EQ(domainChannel->getName(), name);
    ASSERT_EQ(domainChannel->getCreator()->getId(), userId);
    ASSERT_EQ(domainChannel->getCreatedAt(), createdAt);
    ASSERT_EQ(domainChannel->getUpdatedAt(), updatedAt);
}

TEST_F(ChannelMapperTest, givenDomainChannel_shouldMapToPersistenceChannel)
{
    const auto userId = "userId";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto active = true;
    const auto emailVerified = false;
    const auto verificationCode = "verificationCode";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto user = std::make_shared<User>(userId, email, password, nickname, active, emailVerified, verificationCode,
                                             createdAt, updatedAt);

    const auto domainUser = std::make_shared<domain::User>(userId, email, password, nickname, active, emailVerified,
                                                           verificationCode, createdAt, updatedAt);

    const auto id = "id";
    const auto name = "name";

    const auto domainChannel = std::make_shared<domain::Channel>(id, name, domainUser, createdAt, updatedAt);

    EXPECT_CALL(*userMapper, mapToPersistenceUser(domainUser)).WillOnce(Return(user));

    const auto persistenceChannel = channelMapper.mapToPersistenceChannel(domainChannel);

    ASSERT_EQ(persistenceChannel->getId(), id);
    ASSERT_EQ(persistenceChannel->getName(), name);
    ASSERT_EQ(persistenceChannel->getCreator()->getId(), userId);
    ASSERT_EQ(persistenceChannel->getCreatedAt(), createdAt);
    ASSERT_EQ(persistenceChannel->getUpdatedAt(), updatedAt);
}
