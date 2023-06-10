#include "gtest/gtest.h"

#include "HashServiceImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::application;

class HashServiceImplIntegrationTest : public Test
{
public:
    HashServiceImpl hashService;
};

TEST_F(HashServiceImplIntegrationTest, shouldHashData)
{
    const auto data = "123456";

    const auto hashedData = hashService.hash(data);

    const auto hashesHaveSameValue = hashService.compare(data, hashedData);

    ASSERT_TRUE(hashesHaveSameValue);
}
