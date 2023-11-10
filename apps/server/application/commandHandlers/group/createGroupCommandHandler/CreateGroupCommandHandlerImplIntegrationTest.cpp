#include "gtest/gtest.h"

#include "CreateGroupCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/GroupRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class CreateGroupCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        groupTestUtils.truncateTable();
    }

    void TearDown() override
    {
        groupTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    GroupTestUtils groupTestUtils{db};

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<domain::GroupRepository> groupRepository = std::make_shared<GroupRepositoryImpl>(db, groupMapper);

    CreateGroupCommandHandlerImpl createGroupCommandHandler{groupRepository};
};

TEST_F(CreateGroupCommandImplIntegrationTest, createGroup)
{
    const auto [createdGroup] = createGroupCommandHandler.execute();

    const auto foundGroup = groupTestUtils.findById(createdGroup.getId());

    ASSERT_TRUE(foundGroup);
}
