#include "ChannelInvitationTestUtils.h"

#include <format>
#include <utility>

#include "ChannelInvitation.odb.h"
#include "server/infrastructure/database/tables/ChannelInvitationTable.h"

namespace server::tests
{

ChannelInvitationTestUtils::ChannelInvitationTestUtils(std::shared_ptr<odb::pgsql::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      channelInvitationTestFactory{std::make_unique<ChannelInvitationTestFactory>()},
      channelTestFactory{std::make_unique<ChannelTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()}
{
}

void ChannelInvitationTestUtils::persist(const std::shared_ptr<infrastructure::ChannelInvitation>& channelInvitation)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(channelInvitation);

    transaction.commit();
}

std::shared_ptr<infrastructure::ChannelInvitation>
ChannelInvitationTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& senderInput,
                                             const std::shared_ptr<infrastructure::User>& recipientInput,
                                             const std::shared_ptr<infrastructure::Channel>& channelInput)
{
    const auto sender = senderInput ? senderInput : userTestFactory->createPersistentUser();

    const auto recipient = recipientInput ? recipientInput : userTestFactory->createPersistentUser();

    const auto channel = channelInput ? channelInput : channelTestFactory->createPersistentChannel(sender);

    auto channelInvitation =
        channelInvitationTestFactory->createPersistentChannelInvitation(sender, recipient, channel);

    persist(channelInvitation);

    return channelInvitation;
}

std::shared_ptr<infrastructure::ChannelInvitation> ChannelInvitationTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::ChannelInvitation> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::ChannelInvitation> foundChannelInvitation(
        databaseClient->query_one<infrastructure::ChannelInvitation>(query::id == id));

    transaction.commit();

    return foundChannelInvitation;
}

std::shared_ptr<infrastructure::ChannelInvitation> ChannelInvitationTestUtils::find(const std::string& senderId,
                                                                                    const std::string& recipientId,
                                                                                    const std::string& channelId)
{
    typedef odb::query<infrastructure::ChannelInvitation> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::ChannelInvitation> foundChannelInvitation(databaseClient->query_one<infrastructure::ChannelInvitation>(
        query::sender->id == senderId && query::recipient->id == recipientId && query::channel->id == channelId));

    transaction.commit();

    return foundChannelInvitation;
}

void ChannelInvitationTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(std::format("DELETE FROM \"{}\";", infrastructure::channelInvitationTableName));

    transaction.commit();
}

}
