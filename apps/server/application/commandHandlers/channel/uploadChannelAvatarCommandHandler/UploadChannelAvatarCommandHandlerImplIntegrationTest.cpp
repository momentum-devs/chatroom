#include <fstream>

#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "filesystem/GetProjectPath.h"
#include "fmt/format.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/application/services/s3Service/S3ServiceFactory.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "UploadChannelAvatarCommandHandlerImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;
using namespace server::tests;

namespace
{
const std::string resourcesDirectory{fmt::format("{}/resources", common::filesystem::getProjectPath("chatroom"))};
}

class UploadChannelAvatarCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    ChannelTestUtils channelTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    std::shared_ptr<core::ConfigProvider> configProvider = std::make_shared<core::ConfigProvider>();

    const std::string accessKeyId = configProvider->getAwsAccessKeyId();
    const std::string secretAccessKey = configProvider->getAwsSecretAccessKey();
    const std::string s3Endpoint = "http://127.0.0.1:4566";
    const std::string bucketName = configProvider->getAwsBucket();
    const std::string region = configProvider->getAwsRegion();

    std::shared_ptr<S3Service> s3Service = S3ServiceFactory::create({accessKeyId, secretAccessKey, s3Endpoint});

    UploadChannelAvatarCommandHandlerImpl uploadChannelAvatarCommandHandler{channelRepository, s3Service,
                                                                            configProvider};
};

TEST_F(UploadChannelAvatarCommandImplIntegrationTest, givenNotExistingChannel_shouldThrow)
{
    const auto id = faker::String::uuid();

    const auto data = faker::String::alphanumeric(100);

    ASSERT_THROW(uploadChannelAvatarCommandHandler.execute({id, data}), errors::ResourceNotFoundError);
}

TEST_F(UploadChannelAvatarCommandImplIntegrationTest, uploadsChannelAvatarAndUpdatesAvatarUrl)
{
    const auto channel = channelTestUtils.createAndPersist();

    const auto avatarName = faker::Word::noun();

    std::ifstream fileStream{fmt::format("{}/{}", resourcesDirectory, "example_avatar.jpg")};

    std::stringstream buffer;

    buffer << fileStream.rdbuf();

    const auto avatarData = buffer.str();

    uploadChannelAvatarCommandHandler.execute({channel->getId(), avatarData});

    const auto foundChannel = channelTestUtils.findById(channel->getId());

    const auto expectedObjectKey = channel->getId();

    const auto expectedAvatarUrl =
        fmt::format("https://{}.s3-{}.amazonaws.com/avatars/{}", bucketName, region, expectedObjectKey);

    ASSERT_EQ(foundChannel->getAvatarUrl().get(), expectedAvatarUrl);

    const auto actualAvatarData = s3Service->getObject({bucketName, expectedObjectKey});

    EXPECT_EQ(actualAvatarData, avatarData);
}
