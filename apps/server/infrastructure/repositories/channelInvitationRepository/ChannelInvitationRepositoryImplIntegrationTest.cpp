#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.h"
#include "ChannelInvitation.h"
#include "ChannelInvitation.odb.h"
#include "ChannelInvitationRepositoryImpl.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ChannelInvitationRepositoryIntegrationTest : public Test
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

    std::shared_ptr<Channel> createChannel(const std::string& id, const std::string& name, const std::string& creatorId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto channel = std::make_shared<Channel>(id, name, creatorId, currentDate, currentDate);

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

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>();

    std::shared_ptr<ChannelInvitationMapper> channelInvitationMapper =
        std::make_shared<ChannelInvitationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::ChannelInvitationRepository> channelInvitationRepository =
        std::make_shared<server::infrastructure::ChannelInvitationRepositoryImpl>(db, channelInvitationMapper,
                                                                                  userMapper, channelMapper);
};

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldCreateChannelInvitation)
{
    const auto channelInvitationId = faker::String::uuid();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, senderId);

    const auto channelInvitation = channelInvitationRepository->createChannelInvitation(
        {channelInvitationId, userMapper->mapToDomainUser(sender), userMapper->mapToDomainUser(recipient),
         channelMapper->mapToDomainChannel(channel)});

    ASSERT_EQ(channelInvitation.getSender()->getId(), senderId);
    ASSERT_EQ(channelInvitation.getRecipient()->getId(), recipientId);
    ASSERT_EQ(channelInvitation.getChannel()->getId(), channelId);

    typedef odb::query<ChannelInvitation> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<ChannelInvitation> foundChannelInvitation(
            db->query_one<ChannelInvitation>(query::id == channelInvitationId));

        ASSERT_TRUE(foundChannelInvitation);

        transaction.commit();
    }
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldDeleteExistingChannelInvitation)
{
    const auto channelInvitationId = faker::String::uuid();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, senderId);

    const auto channelInvitation = createChannelInvitation(channelInvitationId, sender, recipient, channel);

    const auto domainChannelInvitation = domain::ChannelInvitation{channelInvitation.getId(),
                                                                   userMapper->mapToDomainUser(sender),
                                                                   userMapper->mapToDomainUser(recipient),
                                                                   channelMapper->mapToDomainChannel(channel),
                                                                   channelInvitation.getCreatedAt(),
                                                                   channelInvitation.getUpdatedAt()};

    channelInvitationRepository->deleteChannelInvitation({domainChannelInvitation});

    typedef odb::query<ChannelInvitation> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<ChannelInvitation> foundChannelInvitation(
            db->query_one<ChannelInvitation>(query::id == channelInvitationId));

        ASSERT_FALSE(foundChannelInvitation);

        transaction.commit();
    }
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, delete_givenNonExistingChannelInvitation_shouldThrowError)
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

    const auto channel = createChannel(channelId, name, senderId);

    const auto domainChannelInvitation = domain::ChannelInvitation{channelInvitationId,
                                                                   userMapper->mapToDomainUser(sender),
                                                                   userMapper->mapToDomainUser(recipient),
                                                                   channelMapper->mapToDomainChannel(channel),
                                                                   createdAt,
                                                                   updatedAt};

    ASSERT_ANY_THROW(channelInvitationRepository->deleteChannelInvitation({domainChannelInvitation}));
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldFindChannelInvitationById)
{
    const auto channelInvitationId = faker::String::uuid();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, senderId);

    const auto channelInvitation = createChannelInvitation(channelInvitationId, sender, recipient, channel);

    const auto foundChannelInvitation = channelInvitationRepository->findChannelInvitationById({channelInvitationId});

    ASSERT_TRUE(foundChannelInvitation);
    ASSERT_EQ(foundChannelInvitation->getId(), channelInvitationId);
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldFindChannelInvitationsByRecipientId)
{
    const auto channelInvitationId = faker::String::uuid();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = createChannel(channelId, name, senderId);

    const auto channelInvitation = createChannelInvitation(channelInvitationId, sender, recipient, channel);

    const auto foundChannelInvitations =
        channelInvitationRepository->findChannelInvitationsByRecipientId({recipientId});

    ASSERT_EQ(foundChannelInvitations.size(), 1);
    ASSERT_EQ(foundChannelInvitations[0].getId(), channelInvitation.getId());
}
