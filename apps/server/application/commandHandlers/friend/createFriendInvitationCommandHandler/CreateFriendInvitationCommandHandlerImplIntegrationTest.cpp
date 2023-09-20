#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "CreateFriendInvitationCommandHandlerImpl.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "FriendInvitation.h"
#include "FriendInvitation.odb.h"
#include "Friendship.h"
#include "Friendship.odb.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
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

class CreateFriendInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"friendships\";");
        db->execute("DELETE FROM \"friends_invitations\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"friendships\";");
        db->execute("DELETE FROM \"friends_invitations\";");
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

    Friendship createFriendship(const std::string& id, std::shared_ptr<User> user, std::shared_ptr<User> userFriend)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        Friendship friendship{id, std::move(user), std::move(userFriend), currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(friendship);

        transaction.commit();

        return friendship;
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

    CreateFriendInvitationCommandHandlerImpl createFriendInvitationCommandHandler{friendInvitationRepository,
                                                                                  userRepository,
                                                                                  friendshipRepository};
};

TEST_F(CreateFriendInvitationCommandImplIntegrationTest, createFriendInvitation)
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

    const auto name = faker::Word::noun();

    createFriendInvitationCommandHandler.execute({sender->getId(), recipient->getId()});

    typedef odb::query<FriendInvitation> Query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<FriendInvitation> foundFriendInvitation(
            db->query_one<FriendInvitation>(Query::sender->id == senderId && Query::recipient->id == recipientId));

        ASSERT_TRUE(foundFriendInvitation);

        transaction.commit();
    }
}

TEST_F(CreateFriendInvitationCommandImplIntegrationTest, throwsAnError_whenFriendInvitationAlreadyExists)
{
    const auto friendInvitationId = faker::String::uuid();
    const auto updatedAt = faker::Date::recentDate();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    createFriendInvitation(friendInvitationId, sender, recipient);

    ASSERT_THROW(createFriendInvitationCommandHandler.execute({sender->getId(), recipient->getId()}),errors::OperationNotValidError);
}

TEST_F(CreateFriendInvitationCommandImplIntegrationTest, throwsAnError_whenFriendshipAlreadyExists)
{
    const auto friendshipId = faker::String::uuid();

    const auto senderId = faker::String::uuid();
    const auto recipientId = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient = createUser(recipientId, recipientEmail, password);

    createFriendship(friendshipId, sender, recipient);

    ASSERT_THROW(createFriendInvitationCommandHandler.execute({sender->getId(), recipient->getId()}),errors::OperationNotValidError);
}
