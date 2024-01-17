#include "gtest/gtest.h"

#include "AcceptFriendInvitationCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapper.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/GroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapper.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/UserGroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendInvitationTestUtils/FriendInvitationTestUtils.h"
#include "server/tests/utils/friendshipTestUtils/FriendshipTestUtils.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/userGroupTestUtils/UserGroupTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class AcceptFriendInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendInvitationTestUtils.truncateTable();

        friendshipTestUtils.truncateTable();

        userGroupTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendInvitationTestUtils.truncateTable();

        friendshipTestUtils.truncateTable();

        userGroupTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    FriendshipTestUtils friendshipTestUtils{db};
    FriendInvitationTestUtils friendInvitationTestUtils{db};
    GroupTestUtils groupTestUtils{db};
    UserGroupTestUtils userGroupTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();
    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapperInit =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapper =
        std::make_shared<FriendshipMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapper, userMapper, groupMapper);

    std::shared_ptr<domain::GroupRepository> groupRepository = std::make_shared<GroupRepositoryImpl>(db, groupMapper);

    std::shared_ptr<UserGroupMapper> userGroupMapper = std::make_shared<UserGroupMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::UserGroupRepository> userGroupRepository =
        std::make_shared<UserGroupRepositoryImpl>(db, userGroupMapper, userMapper, groupMapper);

    AcceptFriendInvitationCommandHandlerImpl acceptFriendInvitationCommandHandler{
        friendInvitationRepository, userRepository, friendshipRepository, groupRepository, userGroupRepository};
};

TEST_F(AcceptFriendInvitationCommandImplIntegrationTest, acceptFriendInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    acceptFriendInvitationCommandHandler.execute({recipient->getId(), friendInvitation->getId()});

    const auto foundFriendInvitation = friendInvitationTestUtils.findById(friendInvitation->getId());

    ASSERT_FALSE(foundFriendInvitation);

    const auto foundFriendship = friendshipTestUtils.find(sender->getId(), recipient->getId());

    ASSERT_TRUE(foundFriendship);

    const auto foundUserGroup1 = userGroupTestUtils.find(sender->getId(), foundFriendship->getGroup()->getId());

    ASSERT_TRUE(foundUserGroup1);

    const auto foundUserGroup2 = userGroupTestUtils.find(recipient->getId(), foundFriendship->getGroup()->getId());

    ASSERT_TRUE(foundUserGroup2);
}

TEST_F(AcceptFriendInvitationCommandImplIntegrationTest, givenNotExistingFriendInvitation_shouldThrow)
{
    const auto friendInvitationId = faker::String::uuid();

    const auto recipient = userTestUtils.createAndPersist();

    ASSERT_THROW(acceptFriendInvitationCommandHandler.execute({recipient->getId(), friendInvitationId}),
                 errors::ResourceNotFoundError);
}

TEST_F(AcceptFriendInvitationCommandImplIntegrationTest, givenNotExistingRecipient_shouldThrow)
{
    const auto sender = userTestUtils.createAndPersist();
    const auto recipient = userTestUtils.createAndPersist();

    const auto nonExistingRecipientId = faker::String::uuid();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    ASSERT_THROW(acceptFriendInvitationCommandHandler.execute({nonExistingRecipientId, friendInvitation->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(AcceptFriendInvitationCommandImplIntegrationTest, givenNotRecipientOfFriendInvitation_shouldThrow)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    const auto notRecipientId = faker::String::uuid();

    ASSERT_THROW(acceptFriendInvitationCommandHandler.execute({notRecipientId, friendInvitation->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(AcceptFriendInvitationCommandImplIntegrationTest, givenAlreadyAcceptedFriendInvitation_shouldThrow)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    friendshipTestUtils.createAndPersist(sender, recipient, group);

    ASSERT_THROW(acceptFriendInvitationCommandHandler.execute({recipient->getId(), friendInvitation->getId()}),
                 errors::OperationNotValidError);
}
