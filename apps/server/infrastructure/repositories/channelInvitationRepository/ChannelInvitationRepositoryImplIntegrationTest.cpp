#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "ChannelInvitationRepositoryImpl.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelInvitationTestUtils/ChannelInvitationTestUtils.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ChannelInvitationRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelInvitationTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelInvitationTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    ChannelInvitationTestUtils channelInvitationTestUtils{db};

    UserTestFactory userTestFactory;

    ChannelInvitationTestFactory channelInvitationTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<ChannelInvitationMapper> channelInvitationMapper =
        std::make_shared<ChannelInvitationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<server::domain::ChannelInvitationRepository> channelInvitationRepository =
        std::make_shared<server::infrastructure::ChannelInvitationRepositoryImpl>(db, channelInvitationMapper,
                                                                                  userMapper, channelMapper);
};

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldCreateChannelInvitation)
{
    const auto id = faker::String::uuid();

    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto channelInvitation = channelInvitationRepository->createChannelInvitation(
        {id, userMapper->mapToDomainUser(sender), userMapper->mapToDomainUser(recipient),
         channelMapper->mapToDomainChannel(channel)});

    ASSERT_EQ(channelInvitation.getSender()->getId(), sender->getId());
    ASSERT_EQ(channelInvitation.getRecipient()->getId(), recipient->getId());
    ASSERT_EQ(channelInvitation.getChannel()->getId(), channel->getId());

    const auto foundChannelInvitation = channelInvitationTestUtils.findById(id);

    ASSERT_TRUE(foundChannelInvitation);
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldDeleteExistingChannelInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto channelInvitation = channelInvitationTestUtils.createAndPersist(sender, recipient, channel);

    const auto domainChannelInvitation = channelInvitationMapper->mapToDomainChannelInvitation(*channelInvitation);

    channelInvitationRepository->deleteChannelInvitation({domainChannelInvitation});

    const auto foundChannelInvitation = channelInvitationTestUtils.findById(channelInvitation->getId());

    ASSERT_FALSE(foundChannelInvitation);
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, delete_givenNonExistingChannelInvitation_shouldThrowError)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto domainChannelInvitation = channelInvitationTestFactory.createDomainChannelInvitation(
        userMapper->mapToDomainUser(sender), userMapper->mapToDomainUser(recipient),
        channelMapper->mapToDomainChannel(channel));

    ASSERT_ANY_THROW(channelInvitationRepository->deleteChannelInvitation({*domainChannelInvitation}));
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldFindChannelInvitationById)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto channelInvitation = channelInvitationTestUtils.createAndPersist(sender, recipient, channel);

    const auto foundChannelInvitation =
        channelInvitationRepository->findChannelInvitationById({channelInvitation->getId()});

    ASSERT_TRUE(foundChannelInvitation);
    ASSERT_EQ(foundChannelInvitation->getId(), channelInvitation->getId());
}

TEST_F(ChannelInvitationRepositoryIntegrationTest, shouldFindChannelInvitationsByRecipientId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto channelInvitation = channelInvitationTestUtils.createAndPersist(sender, recipient, channel);

    const auto foundChannelInvitations =
        channelInvitationRepository->findChannelInvitationsByRecipientId({recipient->getId()});

    ASSERT_EQ(foundChannelInvitations.size(), 1);
    ASSERT_EQ(foundChannelInvitations[0].getId(), channelInvitation->getId());
    ASSERT_EQ(foundChannelInvitations[0].getRecipient()->getId(), recipient->getId());
}
