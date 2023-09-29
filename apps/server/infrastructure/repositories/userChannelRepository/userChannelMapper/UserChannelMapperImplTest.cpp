#include "UserChannelMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"

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

    const auto channel = std::make_shared<Channel>(channelId, name, user, createdAt, updatedAt);

    const auto domainChannel = std::make_shared<domain::Channel>(channelId, name, domainUser, createdAt, updatedAt);

    const auto id = faker::String::uuid();

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
