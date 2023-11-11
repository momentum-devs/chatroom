#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "Channel.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/userChannelTestFactory/UserChannelTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"
#include "UserChannel.h"

namespace server::tests
{
class MessageTestUtils
{
public:
    explicit MessageTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::Message>& message);
    std::shared_ptr<infrastructure::Message> createAndPersist(const std::shared_ptr<infrastructure::User>& sender,
                                                              const std::shared_ptr<infrastructure::Channel>& channel);
    std::shared_ptr<infrastructure::Message> createAndPersist(const std::shared_ptr<infrastructure::User>& sender,
                                                              const std::shared_ptr<infrastructure::Group>& group);
    std::shared_ptr<infrastructure::Message> findById(const std::string& id);
    std::shared_ptr<infrastructure::Message> findBySenderAndChannel(const std::string& senderId,
                                                                    const std::string& channelId);
    std::shared_ptr<infrastructure::Message> findBySenderAndGroup(const std::string& senderId,
                                                                  const std::string& groupId);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<MessageTestFactory> messageTestFactory;
    std::unique_ptr<ChannelTestFactory> channelTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
    std::unique_ptr<GroupTestFactory> groupTestFactory;
};
}
