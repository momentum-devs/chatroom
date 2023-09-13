#include <boost/date_time/posix_time/posix_time.hpp>
#include <utility>

#include "gtest/gtest.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "FriendInvitation.h"
#include "FriendInvitation.odb.h"
#include "RejectFriendInvitationCommandHandlerImpl.h"
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

class RejectFriendInvitationCommandImplIntegrationTest : public Test
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

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapper =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapper, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    RejectFriendInvitationCommandHandlerImpl rejectFriendInvitationCommandHandler{friendInvitationRepository,
                                                                                  userRepository};
};

TEST_F(RejectFriendInvitationCommandImplIntegrationTest, rejectFriendInvitation)
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

    rejectFriendInvitationCommandHandler.execute({recipient->getId(), friendInvitation.getId()});

    typedef odb::query<FriendInvitation> FriendInvitationQuery;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<FriendInvitation> foundFriendInvitation(
            db->query_one<FriendInvitation>(FriendInvitationQuery::id == friendInvitationId));

        ASSERT_FALSE(foundFriendInvitation);

        transaction.commit();
    }
}
