#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "Channel.h"
#include "server/tests/factories/channelInvitationTestFactory/ChannelInvitationTestFactory.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/userChannelTestFactory/UserChannelTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"
#include "UserChannel.h"

namespace server::tests
{
class ChannelInvitationTestUtils
{
public:
    explicit ChannelInvitationTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::ChannelInvitation>& channelInvitation);
    std::shared_ptr<infrastructure::ChannelInvitation>
    createAndPersist(const std::shared_ptr<infrastructure::User>& sender,
                     const std::shared_ptr<infrastructure::User>& recipient,
                     const std::shared_ptr<infrastructure::Channel>& channel);
    std::shared_ptr<infrastructure::ChannelInvitation> findById(const std::string& id);
    std::shared_ptr<infrastructure::ChannelInvitation> find(const std::string& senderId, const std::string& recipientId,
                                                            const std::string& channelId);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<ChannelInvitationTestFactory> channelInvitationTestFactory;
    std::unique_ptr<ChannelTestFactory> channelTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
};
}
