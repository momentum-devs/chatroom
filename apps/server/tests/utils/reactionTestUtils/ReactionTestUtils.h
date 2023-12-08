#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "Channel.h"
#include "Reaction.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/reactionTestFactory/ReactionTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"

namespace server::tests
{
class ReactionTestUtils
{
public:
    explicit ReactionTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::Reaction>& reaction);
    std::shared_ptr<infrastructure::Reaction> createAndPersist(const std::shared_ptr<infrastructure::User>& user,
                                                               const std::shared_ptr<infrastructure::Message>& message);
    std::shared_ptr<infrastructure::Reaction> findById(const std::string& id);
    std::shared_ptr<infrastructure::Reaction> find(const std::string& userId, const std::string& messageId);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<ReactionTestFactory> reactionTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
    std::unique_ptr<MessageTestFactory> messageTestFactory;
    std::unique_ptr<ChannelTestFactory> channelTestFactory;
};
}
