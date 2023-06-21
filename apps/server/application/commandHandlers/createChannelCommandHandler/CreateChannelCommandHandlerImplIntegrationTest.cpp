#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "CreateChannelCommandHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;

class CreateChannelCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"channels\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

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

    std::unique_ptr<ChannelMapper> channelMapperInit = std::make_unique<ChannelMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, std::move(channelMapperInit));

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    CreateChannelCommandHandlerImpl createChannelCommandHandler{channelRepository};
};

TEST_F(CreateChannelCommandImplIntegrationTest, registerChannel)
{
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto name = "name";
    const auto creatorId = user.getId();

    const auto [channel] = createChannelCommandHandler.execute({name, creatorId});

    ASSERT_EQ(channel.getName(), name);
    ASSERT_EQ(channel.getCreatorId(), creatorId);
}
