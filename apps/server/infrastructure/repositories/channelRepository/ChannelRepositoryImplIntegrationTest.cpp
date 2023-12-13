#include <odb/sqlite/database.hxx>

#include "gtest/gtest.h"

#include "ChannelRepositoryImpl.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ChannelRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};

    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;

    std::shared_ptr<server::infrastructure::UserMapper> userMapper =
        std::make_shared<server::infrastructure::UserMapperImpl>();

    std::shared_ptr<server::infrastructure::ChannelMapper> channelMapper =
        std::make_shared<server::infrastructure::ChannelMapperImpl>(userMapper);

    std::shared_ptr<server::domain::ChannelRepository> channelRepository =
        std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, channelMapper, userMapper);
};

TEST_F(ChannelRepositoryIntegrationTest, shouldCreateChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = channelRepository->createChannel({channelId, name, userMapper->mapToDomainUser(user)});

    ASSERT_EQ(channel->getId(), channelId);
    ASSERT_EQ(channel->getName(), name);
    ASSERT_EQ(channel->getCreator()->getId(), user->getId());
}

TEST_F(ChannelRepositoryIntegrationTest, shouldDeleteExistingChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto domainChannel = channelMapper->mapToDomainChannel(channel);

    channelRepository->deleteChannel({*domainChannel});

    const auto foundChannel = channelTestUtils.findById(channel->getId());

    ASSERT_FALSE(foundChannel);
}

TEST_F(ChannelRepositoryIntegrationTest, delete_givenNonExistingChannel_shouldThrowError)
{
    const auto user = userTestUtils.createAndPersist();

    const auto domainChannel = channelTestFactory.createDomainChannel(userMapper->mapToDomainUser(user));

    ASSERT_ANY_THROW(channelRepository->deleteChannel({*domainChannel}));
}

TEST_F(ChannelRepositoryIntegrationTest, shouldFindExistingChannelById)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto foundChannel = channelRepository->findChannelById({channel->getId()});

    ASSERT_TRUE(foundChannel);
    ASSERT_EQ(foundChannel->get()->getId(), channel->getId());
}

TEST_F(ChannelRepositoryIntegrationTest, givenNonExistingChannel_shouldNotFindAnyChannelById)
{
    const auto channelId = faker::String::uuid();

    const auto channel = channelRepository->findChannelById({channelId});

    ASSERT_FALSE(channel);
}

TEST_F(ChannelRepositoryIntegrationTest, shouldUpdateExistingChannel)
{
    const auto updatedName = faker::Word::noun();

    const auto channel = channelTestUtils.createAndPersist();

    const auto domainChannel = channelMapper->mapToDomainChannel(channel);

    domainChannel->setName(updatedName);

    channelRepository->updateChannel({*domainChannel});

    const auto updatedUser = channelTestUtils.findById({channel->getId()});

    ASSERT_TRUE(updatedUser);
    ASSERT_EQ((*updatedUser).getName(), updatedName);
}

TEST_F(ChannelRepositoryIntegrationTest, update_givenNonExistingUser_shouldThrowError)
{
    const auto domainUser = userTestFactory.createDomainUser();

    const auto domainChannel = channelTestFactory.createDomainChannel(domainUser);

    ASSERT_ANY_THROW(channelRepository->updateChannel({*domainChannel}));
}
