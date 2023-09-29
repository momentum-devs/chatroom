#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "CreateChannelCommandHandlerImpl.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapper.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"
#include "UserChannel.h"
#include "UserChannel.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class CreateChannelCommandImplIntegrationTest : public Test
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

        User user{id, email, password, email, false, false, "123", currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::UserChannelRepository> userChannelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<AddUserToChannelCommandHandler> addUserToChannelCommandHandler =
        std::make_shared<AddUserToChannelCommandHandlerImpl>(userChannelRepository, userRepository, channelRepository);

    CreateChannelCommandHandlerImpl createChannelCommandHandler{channelRepository, addUserToChannelCommandHandler,
                                                                userRepository};
};

TEST_F(CreateChannelCommandImplIntegrationTest, createChannel)
{
    const auto userId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userPassword = faker::Internet::password();

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();
    const auto creatorId = user.getId();

    const auto [channel] = createChannelCommandHandler.execute({name, creatorId});

    ASSERT_EQ(channel.getName(), name);
    ASSERT_EQ(channel.getCreator()->getId(), creatorId);

    typedef odb::query<UserChannel> Query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<UserChannel> foundUserChannel(
            db->query_one<UserChannel>(Query::user->id == userId && Query::channel->id == channel.getId()));

        ASSERT_TRUE(foundUserChannel);

        transaction.commit();
    }
}
