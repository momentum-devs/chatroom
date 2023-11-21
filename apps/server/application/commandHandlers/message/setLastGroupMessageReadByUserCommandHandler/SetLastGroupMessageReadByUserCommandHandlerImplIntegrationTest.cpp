#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/GroupRepositoryImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/MessageRepositoryImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapper.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/UserGroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "server/tests/utils/userGroupTestUtils/UserGroupTestUtils.h"
#include "SetLastGroupMessageReadByUserCommandHandlerImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class SetLastGroupMessageReadByUserCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userGroupTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userGroupTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    GroupTestFactory groupTestFactory;

    UserTestUtils userTestUtils{db};
    GroupTestUtils groupTestUtils{db};
    MessageTestUtils messageTestUtils{db};
    UserGroupTestUtils userGroupTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<domain::GroupRepository> groupRepository = std::make_shared<GroupRepositoryImpl>(db, groupMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<UserGroupMapper> userGroupMapper = std::make_shared<UserGroupMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::UserGroupRepository> userGroupRepository =
        std::make_shared<UserGroupRepositoryImpl>(db, userGroupMapper, userMapper, groupMapper);

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<server::domain::MessageRepository> messageRepository =
        std::make_shared<server::infrastructure::MessageRepositoryImpl>(db, messageMapper, userMapper, channelMapper,
                                                                        groupMapper);

    SetLastGroupMessageReadByUserCommandHandlerImpl setLastGroupMessageReadByUserCommandHandler{
        groupRepository, userRepository, userGroupRepository, messageRepository};
};

TEST_F(SetLastGroupMessageReadByUserCommandImplIntegrationTest, setLastGroupMessageReadByUser)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto userGroup = userGroupTestUtils.createAndPersist(user, group);

    const auto message = messageTestUtils.createAndPersist(user, group);

    setLastGroupMessageReadByUserCommandHandler.execute({message->getId(), user->getId(), group->getId()});

    const auto updatedUserGroup = userGroupTestUtils.findById(userGroup->getId());

    ASSERT_EQ(updatedUserGroup->getLastReadMessageId().get(), message->getId());
}

TEST_F(SetLastGroupMessageReadByUserCommandImplIntegrationTest, givenNotExistingMessage_shouldThrow)
{
    const auto messageId = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    userGroupTestUtils.createAndPersist(user, group);

    ASSERT_THROW(setLastGroupMessageReadByUserCommandHandler.execute({messageId, user->getId(), group->getId()}),
                 errors::ResourceNotFoundError);
}

TEST_F(SetLastGroupMessageReadByUserCommandImplIntegrationTest, givenNotExistingUser_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, group);

    const auto nonExistingUserId = faker::String::uuid();

    ASSERT_THROW(
        setLastGroupMessageReadByUserCommandHandler.execute({message->getId(), nonExistingUserId, group->getId()}),
        errors::ResourceNotFoundError);
}

TEST_F(SetLastGroupMessageReadByUserCommandImplIntegrationTest, givenNotExistingGroup_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, group);

    const auto nonExistingGroupId = faker::String::uuid();

    ASSERT_THROW(
        setLastGroupMessageReadByUserCommandHandler.execute({message->getId(), user->getId(), nonExistingGroupId}),
        errors::ResourceNotFoundError);
}

TEST_F(SetLastGroupMessageReadByUserCommandImplIntegrationTest, givenUserWhichIsNotMemberOfGroup_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, group);

    ASSERT_THROW(setLastGroupMessageReadByUserCommandHandler.execute({message->getId(), user->getId(), group->getId()}),
                 errors::ResourceNotFoundError);
}
