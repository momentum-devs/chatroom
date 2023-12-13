#include <odb/sqlite/database.hxx>

#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendInvitationTestUtils/FriendInvitationTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class FriendInvitationRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendInvitationTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendInvitationTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    FriendInvitationTestUtils friendInvitationTestUtils{db};

    UserTestFactory userTestFactory;

    FriendInvitationTestFactory friendInvitationTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapper =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<server::domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<server::infrastructure::FriendInvitationRepositoryImpl>(db, friendInvitationMapper,
                                                                                 userMapper);
};

TEST_F(FriendInvitationRepositoryIntegrationTest, shouldCreateFriendInvitation)
{
    const auto friendInvitationId = faker::String::uuid();

    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationRepository->createFriendInvitation(
        {friendInvitationId, userMapper->mapToDomainUser(sender), userMapper->mapToDomainUser(recipient)});

    ASSERT_EQ(friendInvitation.getSender()->getId(), sender->getId());
    ASSERT_EQ(friendInvitation.getRecipient()->getId(), recipient->getId());

    const auto foundFriendInvitation = friendInvitationTestUtils.findById(friendInvitationId);

    ASSERT_TRUE(foundFriendInvitation);
}

TEST_F(FriendInvitationRepositoryIntegrationTest, shouldDeleteExistingFriendInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    const auto domainFriendInvitation = friendInvitationMapper->mapToDomainFriendInvitation(*friendInvitation);

    friendInvitationRepository->deleteFriendInvitation({domainFriendInvitation});

    const auto foundFriendInvitation = friendInvitationTestUtils.findById(friendInvitation->getId());

    ASSERT_FALSE(foundFriendInvitation);
}

TEST_F(FriendInvitationRepositoryIntegrationTest, delete_givenNonExistingFriendInvitation_shouldThrowError)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestFactory.createDomainFriendInvitation(
        userMapper->mapToDomainUser(sender), userMapper->mapToDomainUser(recipient));

    ASSERT_ANY_THROW(friendInvitationRepository->deleteFriendInvitation({*friendInvitation}));
}

TEST_F(FriendInvitationRepositoryIntegrationTest, shouldFindFriendInvitationById)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    const auto foundFriendInvitation =
        friendInvitationRepository->findFriendInvitationById({friendInvitation->getId()});

    ASSERT_TRUE(foundFriendInvitation);
    ASSERT_EQ(foundFriendInvitation->getId(), friendInvitation->getId());
}

TEST_F(FriendInvitationRepositoryIntegrationTest, shouldFindFriendInvitationBySenderAndRecipientIds)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    const auto foundFriendInvitation =
        friendInvitationRepository->findFriendInvitation({sender->getId(), recipient->getId()});

    ASSERT_TRUE(foundFriendInvitation);
    ASSERT_EQ(foundFriendInvitation->getId(), friendInvitation->getId());
}

TEST_F(FriendInvitationRepositoryIntegrationTest, shouldFindFriendInvitationsByRecipientId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    const auto foundFriendInvitations =
        friendInvitationRepository->findFriendInvitationsByRecipientId({recipient->getId()});

    ASSERT_EQ(foundFriendInvitations.size(), 1);
    ASSERT_EQ(foundFriendInvitations[0].getId(), friendInvitation->getId());
    ASSERT_EQ(foundFriendInvitations[0].getRecipient()->getId(), recipient->getId());
}
