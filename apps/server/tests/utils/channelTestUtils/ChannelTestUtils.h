#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "Channel.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

namespace server::tests
{
class ChannelTestUtils
{
public:
    explicit ChannelTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::Channel>& channel);
    std::shared_ptr<infrastructure::Channel>
    createAndPersist(const std::shared_ptr<infrastructure::User>& user = nullptr);
    std::shared_ptr<infrastructure::Channel> findById(const std::string& id);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<ChannelTestFactory> channelTestFactory;
    std::unique_ptr<UserTestUtils> userTestUtils;
};
}
