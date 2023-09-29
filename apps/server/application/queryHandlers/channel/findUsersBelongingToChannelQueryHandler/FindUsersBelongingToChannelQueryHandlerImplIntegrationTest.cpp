#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "Channel.h"
#include "Channel.odb.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "FindUsersBelongingToChannelQueryHandlerImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "User.h"
#include "User.odb.h"
#include "UserChannel.h"
#include "UserChannel.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class FindUsersBelongingToChannelQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users_channels\";");
        db->execute("DELETE FROM \"channels\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users_channels\";");
        db->execute("DELETE FROM \"channels\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    std::shared_ptr<User> createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto user = std::make_shared<User>(id, email, password, email, false, false, "123", currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    std::shared_ptr<Channel> createChannel(const std::string& id, const std::string& name,
                                           const std::shared_ptr<User>& creator)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto channel = std::make_shared<Channel>(id, name, creator, currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();

        return channel;
    }

    UserChannel createUserChannel(const std::string& id, std::shared_ptr<User> user, std::shared_ptr<Channel> channel)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        UserChannel userChannel{id, std::move(user), std::move(channel), currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(userChannel);

        transaction.commit();

        return userChannel;
    }

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::UserChannelRepository> channelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    FindUsersBelongingToChannelQueryHandlerImpl findUsersBelongingToChannelQueryHandler{channelRepository};
};

TEST_F(FindUsersBelongingToChannelQueryHandlerImplIntegrationTest, findUsersChannelsByChannelId)
{
    const auto userId1 = faker::String::uuid();
    const auto userEmail1 = faker::Internet::email();
    const auto userPassword1 = faker::Internet::password();

    const auto user1 = createUser(userId1, userEmail1, userPassword1);

    const auto channelId1 = faker::String::uuid();
    const auto name1 = faker::Word::noun();

    const auto channel1 = createChannel(channelId1, name1, user1);

    const auto userId2 = faker::String::uuid();
    const auto userEmail2 = faker::Internet::email();
    const auto userPassword2 = faker::Internet::password();

    const auto user2 = createUser(userId2, userEmail2, userPassword2);

    const auto channelId2 = faker::String::uuid();
    const auto name2 = faker::Word::noun();

    const auto channel2 = createChannel(channelId2, name2, user2);

    const auto userUserId1 = faker::String::uuid();
    const auto userUserId2 = faker::String::uuid();

    const auto userChannel1 = createUserChannel(userUserId1, user1, channel1);
    const auto userChannel2 = createUserChannel(userUserId2, user2, channel2);

    const auto [users] = findUsersBelongingToChannelQueryHandler.execute({channelId2});

    ASSERT_EQ(users.size(), 1);
    ASSERT_EQ(users[0].getId(), userId2);
}
