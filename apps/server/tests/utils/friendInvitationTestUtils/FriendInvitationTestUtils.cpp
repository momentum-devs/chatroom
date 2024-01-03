#include "FriendInvitationTestUtils.h"

#include "fmt/format.h"
#include <utility>

#include "FriendInvitation.odb.h"
#include "server/infrastructure/database/tables/FriendInvitationTable.h"

namespace server::tests
{

FriendInvitationTestUtils::FriendInvitationTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      friendInvitationTestFactory{std::make_unique<FriendInvitationTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()}
{
}

void FriendInvitationTestUtils::persist(const std::shared_ptr<infrastructure::FriendInvitation>& friendInvitation)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(friendInvitation);

    transaction.commit();
}

std::shared_ptr<infrastructure::FriendInvitation>
FriendInvitationTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& senderInput,
                                            const std::shared_ptr<infrastructure::User>& recipientInput)
{
    const auto sender = senderInput ? senderInput : userTestFactory->createPersistentUser();

    const auto recipient = recipientInput ? recipientInput : userTestFactory->createPersistentUser();

    auto friendInvitation = friendInvitationTestFactory->createPersistentFriendInvitation(sender, recipient);

    persist(friendInvitation);

    return friendInvitation;
}

std::shared_ptr<infrastructure::FriendInvitation> FriendInvitationTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::FriendInvitation> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::FriendInvitation> foundFriendInvitation(
        databaseClient->query_one<infrastructure::FriendInvitation>(query::id == id));

    transaction.commit();

    return foundFriendInvitation;
}

std::shared_ptr<infrastructure::FriendInvitation> FriendInvitationTestUtils::find(const std::string& senderId,
                                                                                  const std::string& recipientId)
{
    typedef odb::query<infrastructure::FriendInvitation> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::FriendInvitation> foundFriendInvitation(
        databaseClient->query_one<infrastructure::FriendInvitation>(query::sender->id == senderId &&
                                                                    query::recipient->id == recipientId));

    transaction.commit();

    return foundFriendInvitation;
}

void FriendInvitationTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(fmt::format("DELETE FROM \"{}\";", infrastructure::friendInvitationTableName));

    transaction.commit();
}

}
