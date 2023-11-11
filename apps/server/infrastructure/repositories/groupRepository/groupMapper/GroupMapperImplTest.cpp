#include "GroupMapperImpl.h"

#include "gtest/gtest.h"

#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class GroupMapperTest : public Test
{
public:
    GroupTestFactory groupTestFactory;

    GroupMapperImpl groupMapper;
};

TEST_F(GroupMapperTest, givenPersistenceGroup_shouldMapToDomainGroup)
{
    const auto group = groupTestFactory.createPersistentGroup();

    const auto domainGroup = groupMapper.mapToDomainGroup(group);

    ASSERT_EQ(domainGroup->getId(), group->getId());
    ASSERT_EQ(domainGroup->getCreatedAt(), group->getCreatedAt());
}

TEST_F(GroupMapperTest, givenDomainGroup_shouldMapToPersistenceGroup)
{
    const auto domainGroup = groupTestFactory.createDomainGroup();

    const auto persistenceGroup = groupMapper.mapToPersistenceGroup(domainGroup);

    ASSERT_EQ(persistenceGroup->getId(), domainGroup->getId());
    ASSERT_EQ(persistenceGroup->getCreatedAt(), domainGroup->getCreatedAt());
}
