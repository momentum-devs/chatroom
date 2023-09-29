#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "Channel.h"
#include "Channel.odb.h"
#include "ChannelInvitation.h"
#include "ChannelInvitation.odb.h"
#include "CreateChannelInvitationCommandHandlerImpl.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class CreateChannelInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"channels_invitations\";");
        db->execute("DELETE FROM \"channels\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"channels_invitations\";");
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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<ChannelInvitationMapper> channelInvitationMapperInit =
        std::make_shared<ChannelInvitationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository =
        std::make_shared<ChannelInvitationRepositoryImpl>(db, channelInvitationMapperInit, userMapper, channelMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    CreateChannelInvitationCommandHandlerImpl createChannelInvitationCommandHandler{channelInvitationRepository,
                                                                                    userRepository, channelRepository};
};

TEST_F(CreateChannelInvitationCommandImplIntegrationTest, createChannelInvitation)
{
    const auto channelInvitationId = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, sender);

    createChannelInvitationCommandHandler.execute({sender->getId(), recipient->getId(), channel->getId()});

    typedef odb::query<ChannelInvitation> Query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<ChannelInvitation> foundChannelInvitation(db->query_one<ChannelInvitation>(
            Query::sender->id == senderId && Query::recipient->id == recipientId && Query::channel->id == channelId));

        ASSERT_TRUE(foundChannelInvitation);

        transaction.commit();
    }
}
