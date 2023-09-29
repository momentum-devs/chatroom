#include "ChannelMapperImpl.h"

#include "gtest/gtest.h"

#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"

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
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = std::make_shared<User>(userId, email, password, nickname, active, emailVerified, verificationCode,
                                             createdAt, updatedAt);

    const auto domainUser = std::make_shared<domain::User>(userId, email, password, nickname, active, emailVerified,
                                                           verificationCode, createdAt, updatedAt);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto persistenceChannel = std::make_shared<Channel>(channelId, name, user, createdAt, updatedAt);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    const auto domainChannel = channelMapper.mapToDomainChannel(persistenceChannel);

    ASSERT_EQ(domainChannel->getId(), channelId);
    ASSERT_EQ(domainChannel->getName(), name);
    ASSERT_EQ(domainChannel->getCreator()->getId(), userId);
    ASSERT_EQ(domainChannel->getCreatedAt(), createdAt);
    ASSERT_EQ(domainChannel->getUpdatedAt(), updatedAt);
}

TEST_F(ChannelMapperTest, givenDomainChannel_shouldMapToPersistenceChannel)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = std::make_shared<User>(userId, email, password, nickname, active, emailVerified, verificationCode,
                                             createdAt, updatedAt);

    const auto domainUser = std::make_shared<domain::User>(userId, email, password, nickname, active, emailVerified,
                                                           verificationCode, createdAt, updatedAt);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto domainChannel = std::make_shared<domain::Channel>(channelId, name, domainUser, createdAt, updatedAt);

    EXPECT_CALL(*userMapper, mapToPersistenceUser(domainUser)).WillOnce(Return(user));

    const auto persistenceChannel = channelMapper.mapToPersistenceChannel(domainChannel);

    ASSERT_EQ(persistenceChannel->getId(), channelId);
    ASSERT_EQ(persistenceChannel->getName(), name);
    ASSERT_EQ(persistenceChannel->getCreator()->getId(), userId);
    ASSERT_EQ(persistenceChannel->getCreatedAt(), createdAt);
    ASSERT_EQ(persistenceChannel->getUpdatedAt(), updatedAt);
}
