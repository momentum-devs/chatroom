#include <boost/date_time/posix_time/posix_time.hpp>
#include <utility>

#include "gtest/gtest.h"

#include "AcceptChannelInvitationCommandHandlerImpl.h"
#include "Channel.h"
#include "Channel.odb.h"
#include "ChannelInvitation.h"
#include "ChannelInvitation.odb.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.h"
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

class AcceptChannelInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"channels_invitations\";");
        db->execute("DELETE FROM \"users_channels\";");
        db->execute("DELETE FROM \"channels\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"channels_invitations\";");
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

    ChannelInvitation createChannelInvitation(const std::string& id, std::shared_ptr<User> sender,
                                              std::shared_ptr<User> recipient, std::shared_ptr<Channel> channel)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        ChannelInvitation channelInvitation{id,          std::move(sender), std::move(recipient), std::move(channel),
                                            currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(channelInvitation);

        transaction.commit();

        return channelInvitation;
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

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::UserChannelRepository> userChannelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    std::shared_ptr<application::AddUserToChannelCommandHandler> addUserToChannelCommandHandler =
        std::make_shared<AddUserToChannelCommandHandlerImpl>(userChannelRepository, userRepository, channelRepository);

    AcceptChannelInvitationCommandHandlerImpl acceptChannelInvitationCommandHandler{
        channelInvitationRepository, userRepository, addUserToChannelCommandHandler};
};

TEST_F(AcceptChannelInvitationCommandImplIntegrationTest, acceptChannelInvitation)
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

    const auto channelInvitation = createChannelInvitation(channelInvitationId, sender, recipient, channel);

    acceptChannelInvitationCommandHandler.execute({recipient->getId(), channelInvitation.getId()});

    typedef odb::query<ChannelInvitation> ChannelInvitationQuery;
    typedef odb::query<UserChannel> UserChannelQuery;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<ChannelInvitation> foundChannelInvitation(
            db->query_one<ChannelInvitation>(ChannelInvitationQuery::id == channelInvitationId));

        ASSERT_FALSE(foundChannelInvitation);

        std::shared_ptr<UserChannel> foundUserChannel(db->query_one<UserChannel>(
            UserChannelQuery::user->id == recipientId && UserChannelQuery::channel->id == channelId));

        ASSERT_TRUE(foundUserChannel);

        transaction.commit();
    }
}
