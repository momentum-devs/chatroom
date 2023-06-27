#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "../../errors/ResourceNotFoundError.h"
#include "DeleteUserChannelCommandHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/UserChannelRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"
#include "UserChannel.h"
#include "UserChannel.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class DeleteUserChannelCommandImplIntegrationTest : public Test
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

    UserChannel createUserChannel(const std::string& id, const std::string& name, const std::string& creatorId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        UserChannel channel{id, name, creatorId, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();

        return channel;
    }

    std::unique_ptr<UserChannelMapper> channelMapperInit = std::make_unique<UserChannelMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::UserChannelRepository> channelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, std::move(channelMapperInit));

    DeleteUserChannelCommandHandlerImpl deleteUserChannelCommandHandler{channelRepository};
};

TEST_F(DeleteUserChannelCommandImplIntegrationTest, givenExistingUserChannel_shouldDeleteUserChannel)
{
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = "channelId";
    const auto name = "name";
    const auto creatorId = user.getId();

    const auto channel = createUserChannel(channelId, name, creatorId);

    deleteUserChannelCommandHandler.execute({channel.getId()});

    typedef odb::query<UserChannel> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<UserChannel> foundUserChannel(db->query_one<UserChannel>(query::id == channelId));

        ASSERT_FALSE(foundUserChannel);

        transaction.commit();
    }
}

TEST_F(DeleteUserChannelCommandImplIntegrationTest, givenNonExistingUserChannel_shouldThrow)
{
    const auto channelId = "channelId";

    ASSERT_THROW(deleteUserChannelCommandHandler.execute({channelId}), errors::ResourceNotFoundError);
}
