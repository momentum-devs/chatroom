#include "gtest/gtest.h"

#include "CreateFriendshipCommandHandlerImpl.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapperImpl.h"
#include "server/infrastructure/repositories/conversationRepository/ConversationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/conversationTestUtils/ConversationTestUtils.h"
#include "server/tests/utils/friendshipTestUtils/FriendshipTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class CreateFriendshipCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendshipTestUtils.truncateTable();

        userTestUtils.truncateTable();

        conversationTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendshipTestUtils.truncateTable();

        userTestUtils.truncateTable();

        conversationTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    FriendshipTestUtils friendshipTestUtils{db};
    ConversationTestUtils conversationTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();
    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<ConversationMapper> conversationMapper =
        std::make_shared<ConversationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::ConversationRepository> conversationRepository =
        std::make_shared<ConversationRepositoryImpl>(db, conversationMapper, userMapper, channelMapper);

    CreateFriendshipCommandHandlerImpl createFriendshipCommandHandler{friendshipRepository, userRepository,
                                                                      conversationRepository};
};

TEST_F(CreateFriendshipCommandImplIntegrationTest, createFriendship)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    createFriendshipCommandHandler.execute({user->getId(), userFriend->getId()});

    const auto foundFriendship = friendshipTestUtils.find(user->getId(), userFriend->getId());

    ASSERT_TRUE(foundFriendship);

    const auto foundConversation = conversationTestUtils.findByUsers(user->getId(), userFriend->getId());

    ASSERT_TRUE(foundConversation);
}

TEST_F(CreateFriendshipCommandImplIntegrationTest, throwsAnError_whenFriendshipAlreadyExists)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    friendshipTestUtils.createAndPersist(user, userFriend);

    ASSERT_THROW(createFriendshipCommandHandler.execute({user->getId(), userFriend->getId()}),
                 errors::OperationNotValidError);
}
