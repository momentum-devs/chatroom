#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "FindReceivedFriendInvitationsQueryHandlerImpl.h"
#include "FriendInvitation.h"
#include "FriendInvitation.odb.h"
#include "server/application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandlerImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class FindReceivedFriendInvitationsQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"friends_invitations\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapperInit =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    FindReceivedFriendInvitationsQueryHandlerImpl findReceivedFriendInvitationsQueryHandler{friendInvitationRepository};
};

TEST_F(FindReceivedFriendInvitationsQueryHandlerImplIntegrationTest, findFriendInvitationsByRecipientId)
{
    const auto friendInvitationId1 = faker::String::uuid();
    const auto friendInvitationId2 = faker::String::uuid();

    const auto senderId = faker::String::uuid();
    const auto recipientId1 = faker::String::uuid();
    const auto recipientId2 = faker::String::uuid();
    const auto senderEmail = faker::Internet::email();
    const auto recipientEmail1 = faker::Internet::email();
    const auto recipientEmail2 = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto sender = createUser(senderId, senderEmail, password);

    const auto recipient1 = createUser(recipientEmail1, recipientEmail1, password);

    const auto recipient2 = createUser(recipientEmail2, recipientEmail2, password);

    createFriendInvitation(friendInvitationId1, sender, recipient1);
    createFriendInvitation(friendInvitationId2, sender, recipient2);

    const auto [friendInvitations] = findReceivedFriendInvitationsQueryHandler.execute({recipient1->getId()});

    ASSERT_EQ(friendInvitations.size(), 1);
    ASSERT_EQ(friendInvitations[0].getId(), friendInvitationId1);
}
