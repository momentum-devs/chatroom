#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "Channel.h"
#include "Channel.odb.h"
#include "FindUsersChannelsByChannelIdQueryHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"
#include "UserChannel.h"
#include "UserChannel.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class FindUsersChannelsByChannelIdQueryImplIntegrationTest : public Test
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

    User createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        User user{id, email, password, email, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    Channel createChannel(const std::string& id, const std::string& name, const std::string& creatorId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        Channel channel{id, name, creatorId, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();

        return channel;
    }

    UserChannel createUserChannel(const std::string& id, const std::string& userId, const std::string& channelId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        UserChannel userChannel{id, userId, channelId, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(userChannel);

        transaction.commit();

        return userChannel;
    }

    std::unique_ptr<UserChannelMapper> channelMapperInit = std::make_unique<UserChannelMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::UserChannelRepository> channelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, std::move(channelMapperInit));

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    FindUsersChannelsByChannelIdQueryHandlerImpl findUsersChannelsByIdQueryHandler{channelRepository};
};

TEST_F(FindUsersChannelsByChannelIdQueryImplIntegrationTest, findUsersChannelsByChannelId)
{
    const auto userId1 = "userId1";
    const auto userEmail1 = "email1@gmail.com";
    const auto userPassword1 = "password";

    const auto user1 = createUser(userId1, userEmail1, userPassword1);

    const auto channelId1 = "channelId1";
    const auto name1 = "name";
    const auto creatorId1 = user1.getId();

    const auto channel1 = createChannel(channelId1, name1, creatorId1);

    const auto userId2 = "userId2";
    const auto userEmail2 = "email2@gmail.com";
    const auto userPassword2 = "password";

    const auto user2 = createUser(userId2, userEmail2, userPassword2);

    const auto channelId2 = "channelId2";
    const auto name2 = "name";
    const auto creatorId2 = user2.getId();

    const auto channel2 = createChannel(channelId2, name2, creatorId2);

    const auto userChannelId1 = "userChannelId1";
    const auto userChannelId2 = "userChannelId2";

    const auto userChannel1 = createUserChannel(userChannelId1, userId1, channelId1);
    const auto userChannel2 = createUserChannel(userChannelId2, userId2, channelId2);

    const auto [usersChannels] = findUsersChannelsByIdQueryHandler.execute({channelId2});

    ASSERT_EQ(usersChannels.size(), 1);
    ASSERT_EQ(usersChannels[0].getId(), userChannelId1);
}
