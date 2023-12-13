#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>

#include "Channel.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/userChannelTestFactory/UserChannelTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"
#include "UserChannel.h"

namespace server::tests
{
class UserChannelTestUtils
{
public:
    explicit UserChannelTestUtils(std::shared_ptr<odb::sqlite::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::UserChannel>& userChannel);
    std::shared_ptr<infrastructure::UserChannel>
    createAndPersist(const std::shared_ptr<infrastructure::User>& user,
                     const std::shared_ptr<infrastructure::Channel>& channel);
    std::shared_ptr<infrastructure::UserChannel> findById(const std::string& id);
    std::shared_ptr<infrastructure::UserChannel> find(const std::string& userId, const std::string& channelId);
    void truncateTable();

private:
    std::shared_ptr<odb::sqlite::database> databaseClient;
    std::unique_ptr<UserChannelTestFactory> userChannelTestFactory;
    std::unique_ptr<ChannelTestFactory> channelTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
};
}
