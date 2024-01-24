#include <fstream>

#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "filesystem/GetProjectPath.h"
#include "fmt/format.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/application/services/s3Service/S3ServiceFactory.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "UploadUserAvatarCommandHandlerImpl.h"

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

class UploadUserAvatarCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<core::ConfigProvider> configProvider = std::make_shared<core::ConfigProvider>();

    const std::string accessKeyId = configProvider->getAwsAccessKeyId();
    const std::string secretAccessKey = configProvider->getAwsSecretAccessKey();
    const std::string s3Endpoint = "http://127.0.0.1:4566";
    const std::string bucketName = configProvider->getAwsBucket();
    const std::string region = configProvider->getAwsRegion();

    std::shared_ptr<S3Service> s3Service = S3ServiceFactory::create({accessKeyId, secretAccessKey, s3Endpoint});

    UploadUserAvatarCommandHandlerImpl uploadUserAvatarCommandHandler{userRepository, s3Service, configProvider};
};

TEST_F(UploadUserAvatarCommandImplIntegrationTest, givenNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();

    const auto avatarName = faker::Word::noun();

    const auto data = faker::String::alphanumeric(100);

    ASSERT_THROW(uploadUserAvatarCommandHandler.execute({id, avatarName, data}), errors::ResourceNotFoundError);
}

TEST_F(UploadUserAvatarCommandImplIntegrationTest, uploadsUserAvatarAndUpdatesAvatarUrl)
{
    const auto user = userTestUtils.createAndPersist();

    const auto avatarName = faker::Word::noun();

    std::ifstream fileStream{fmt::format("{}/{}", resourcesDirectory, "example_avatar.jpg")};

    std::stringstream buffer;

    buffer << fileStream.rdbuf();

    const auto avatarData = buffer.str();

    uploadUserAvatarCommandHandler.execute({user->getId(), avatarName, avatarData});

    const auto foundUser = userTestUtils.findById(user->getId());

    const auto expectedAvatarUrl =
        fmt::format("https://{}.s3-{}.amazonaws.com/avatars/{}", bucketName, region, user->getId() + avatarName);

    ASSERT_EQ(foundUser->getAvatarUrl().get(), expectedAvatarUrl);

    const auto expectedObjectKey = user->getId() + avatarName;

    const auto actualAvatarData = s3Service->getObject({bucketName, expectedObjectKey});

    EXPECT_EQ(actualAvatarData, avatarData);
}
