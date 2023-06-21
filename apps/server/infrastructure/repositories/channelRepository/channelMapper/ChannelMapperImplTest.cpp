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

TEST_F(ChannelMapperTest, givenChannelModel_shouldMapToDomainChannel)
{
    const auto id = "id";
    const auto name = "name";
    const auto creatorId = "creatorId";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel channelModel{id, name, creatorId, createdAt, updatedAt};

    const auto domainChannel = userMapper.mapToDomainChannel(channelModel);

    ASSERT_EQ(domainChannel.getId(), id);
    ASSERT_EQ(domainChannel.getName(), name);
    ASSERT_EQ(domainChannel.getCreatorId(), creatorId);
    ASSERT_EQ(domainChannel.getCreatedAt(), createdAt);
    ASSERT_EQ(domainChannel.getUpdatedAt(), updatedAt);
}
