#include "ChannelMapperImpl.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ChannelMapperTest : public Test
{
public:
    ChannelMapperImpl userMapper;
};

TEST_F(ChannelMapperTest, givenPersistenceChannel_shouldMapToDomainChannel)
{
    const auto id = "id";
    const auto name = "name";
    const auto creatorId = "creatorId";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto persistenceChannel = std::make_shared<Channel>(id, name, creatorId, createdAt, updatedAt);

    const auto domainChannel = userMapper.mapToDomainChannel(persistenceChannel);

    ASSERT_EQ(domainChannel->getId(), id);
    ASSERT_EQ(domainChannel->getName(), name);
    ASSERT_EQ(domainChannel->getCreatorId(), creatorId);
    ASSERT_EQ(domainChannel->getCreatedAt(), createdAt);
    ASSERT_EQ(domainChannel->getUpdatedAt(), updatedAt);
}

TEST_F(ChannelMapperTest, givenDomainChannel_shouldMapToPersistenceChannel)
{
    const auto id = "id";
    const auto name = "name";
    const auto creatorId = "creatorId";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto domainChannel = std::make_shared<domain::Channel>(id, name, creatorId, createdAt, updatedAt);

    const auto persistenceChannel = userMapper.mapToPersistenceChannel(domainChannel);

    ASSERT_EQ(persistenceChannel->getId(), id);
    ASSERT_EQ(persistenceChannel->getName(), name);
    ASSERT_EQ(persistenceChannel->getCreatorId(), creatorId);
    ASSERT_EQ(persistenceChannel->getCreatedAt(), createdAt);
    ASSERT_EQ(persistenceChannel->getUpdatedAt(), updatedAt);
}
