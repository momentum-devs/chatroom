#include <utility>

#include "gtest/gtest.h"

#include "AcceptFriendInvitationCommandHandlerImpl.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "FriendInvitation.h"
#include "FriendInvitation.odb.h"
#include "Friendship.h"
#include "Friendship.odb.h"
#include "server/application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandlerImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapper.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class AcceptFriendInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"friends_invitations\";");
        db->execute("DELETE FROM \"friendships\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"friends_invitations\";");
        db->execute("DELETE FROM \"friendships\";");
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

    FriendInvitation createFriendInvitation(const std::string& id, std::shared_ptr<User> sender,
                                            std::shared_ptr<User> recipient)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        FriendInvitation friendInvitation{id, std::move(sender), std::move(recipient), currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(friendInvitation);

        transaction.commit();

        return friendInvitation;
    }

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapperInit =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper);

    std::shared_ptr<application::CreateFriendshipCommandHandler> createFriendshipCommandHandler =
        std::make_shared<CreateFriendshipCommandHandlerImpl>(friendshipRepository, userRepository);

    AcceptFriendInvitationCommandHandlerImpl acceptFriendInvitationCommandHandler{
        friendInvitationRepository, userRepository, createFriendshipCommandHandler};
};

TEST_F(AcceptFriendInvitationCommandImplIntegrationTest, acceptFriendInvitation)
{
    const auto friendInvitationId = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    const auto friendInvitation = createFriendInvitation(friendInvitationId, sender, recipient);

    acceptFriendInvitationCommandHandler.execute({recipient->getId(), friendInvitation.getId()});

    typedef odb::query<FriendInvitation> FriendInvitationQuery;
    typedef odb::query<Friendship> FriendshipQuery;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<FriendInvitation> foundFriendInvitation(
            db->query_one<FriendInvitation>(FriendInvitationQuery::id == friendInvitationId));

        ASSERT_FALSE(foundFriendInvitation);

        std::shared_ptr<Friendship> foundFriendship(db->query_one<Friendship>(
            FriendshipQuery::user->id == senderId && FriendshipQuery::user_friend->id == recipientId));

        ASSERT_TRUE(foundFriendship);

        transaction.commit();
    }
}
