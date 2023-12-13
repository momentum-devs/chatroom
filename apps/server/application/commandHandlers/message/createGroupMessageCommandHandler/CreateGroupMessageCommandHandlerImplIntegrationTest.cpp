#include "gtest/gtest.h"

#include "CreateGroupMessageCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
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
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class CreateGroupMessageCommandImplIntegrationTest : public Test
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

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

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

    CreateGroupMessageCommandHandlerImpl createGroupMessageCommandHandler{groupRepository, userRepository,
                                                                          userGroupRepository, messageRepository};
};

TEST_F(CreateGroupMessageCommandImplIntegrationTest, createGroupMessage)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    userGroupTestUtils.createAndPersist(sender, group);

    const auto content = faker::Word::noun();

    const auto [createdGroup] = createGroupMessageCommandHandler.execute({content, sender->getId(), group->getId()});

    ASSERT_EQ(createdGroup.getContent(), content);

    ASSERT_EQ(createdGroup.getSender()->getId(), sender->getId());

    ASSERT_EQ(createdGroup.getGroup()->getId(), group->getId());
}

TEST_F(CreateGroupMessageCommandImplIntegrationTest, givenSenderWhichIsNotMemberOfGroup_shouldThrow)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto content = faker::Word::noun();

    ASSERT_THROW(createGroupMessageCommandHandler.execute({content, sender->getId(), group->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(CreateGroupMessageCommandImplIntegrationTest, givenNotExistingUser_shouldThrow)
{
    const auto senderId = faker::String::uuid();

    const auto group = groupTestUtils.createAndPersist();

    const auto content = faker::Word::noun();

    ASSERT_THROW(createGroupMessageCommandHandler.execute({content, senderId, group->getId()}),
                 errors::ResourceNotFoundError);
}

TEST_F(CreateGroupMessageCommandImplIntegrationTest, givenNotExistingGroup_shouldThrow)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto groupId = faker::String::uuid();

    const auto content = faker::Word::noun();

    ASSERT_THROW(createGroupMessageCommandHandler.execute({content, sender->getId(), groupId}),
                 errors::ResourceNotFoundError);
}
