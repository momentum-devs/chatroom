#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "Channel.h"
#include "Conversation.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/conversationTestFactory/ConversationTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"

namespace server::tests
{
class ConversationTestUtils
{
public:
    explicit ConversationTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::Conversation>& conversation);
    std::shared_ptr<infrastructure::Conversation>
    createAndPersist(const std::shared_ptr<infrastructure::User>& user,
                     const std::shared_ptr<infrastructure::User>& recipient);
    std::shared_ptr<infrastructure::Conversation>
    createAndPersist(const std::shared_ptr<infrastructure::Channel>& channel);
    std::shared_ptr<infrastructure::Conversation> findById(const std::string& id);
    std::shared_ptr<infrastructure::Conversation> findByUsers(const std::string& userId,
                                                              const std::string& recipientId);
    std::shared_ptr<infrastructure::Conversation> findByChannelId(const std::string& channelId);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<ConversationTestFactory> conversationTestFactory;
    std::unique_ptr<ChannelTestFactory> channelTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
};
}
