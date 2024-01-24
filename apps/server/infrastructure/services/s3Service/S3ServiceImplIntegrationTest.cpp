#include <fstream>

#include "gtest/gtest.h"

#include "faker-cxx/Lorem.h"
#include "faker-cxx/Word.h"
#include "filesystem/GetProjectPath.h"
#include "fmt/format.h"
#include "S3ServiceImpl.h"
#include "server/application/services/s3Service/S3ServiceFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::application;

namespace
{
const std::string resourcesDirectory{fmt::format("{}/resources", common::filesystem::getProjectPath("chatroom"))};
}

class S3ServiceImplIntegrationTest : public Test
{
public:
    const std::string awsAccountId = "test";
    const std::string awsAccountSecretKey = "test";
    const std::string bucketName = "avatars";
    const std::string existingObjectKey1 = "example_avatar.jpg";
    const std::string existingObjectKey2 = "example_avatar.jpg";
    const std::string nonExistingObjectKey = "another_avatar.jpg";
    const std::string s3Endpoint = "http://127.0.0.1:4566";

    const std::shared_ptr<S3Service> s3Service =
        S3ServiceFactory::create({awsAccountId, awsAccountSecretKey, s3Endpoint});
};

TEST_F(S3ServiceImplIntegrationTest, shouldGetObject)
{
    const auto object = s3Service->getObject({bucketName, existingObjectKey1});

    EXPECT_FALSE(object.empty());
}

TEST_F(S3ServiceImplIntegrationTest, givenNonExistingObjectKey_shouldThrow)
{
    EXPECT_THROW(s3Service->getObject({bucketName, nonExistingObjectKey}), std::runtime_error);
}

TEST_F(S3ServiceImplIntegrationTest, shouldPutObject)
{
    const auto objectKey = faker::Word::noun();

    std::ifstream fileStream{fmt::format("{}/{}", resourcesDirectory, existingObjectKey1)};

    std::stringstream buffer;

    buffer << fileStream.rdbuf();

    const auto objectData = buffer.str();

    s3Service->putObject({bucketName, objectKey, objectData});

    const auto object = s3Service->getObject({bucketName, objectKey});

    EXPECT_EQ(object, objectData);
}

TEST_F(S3ServiceImplIntegrationTest, shouldReplaceExistingObject)
{
    const auto objectKey = faker::Word::noun();

    std::ifstream fileStream1{fmt::format("{}/{}", resourcesDirectory, existingObjectKey1)};

    std::stringstream buffer1;

    buffer1 << fileStream1.rdbuf();

    const auto objectData1 = buffer1.str();

    std::ifstream fileStream2{fmt::format("{}/{}", resourcesDirectory, existingObjectKey2)};

    std::stringstream buffer2;

    buffer1 << fileStream2.rdbuf();

    const auto objectData2 = buffer2.str();

    s3Service->putObject({bucketName, objectKey, objectData1});

    s3Service->putObject({bucketName, objectKey, objectData2});

    const auto object = s3Service->getObject({bucketName, objectKey});

    EXPECT_EQ(object, objectData2);
}

TEST_F(S3ServiceImplIntegrationTest, givenExistingObject_shouldReturnTrue)
{
    const auto objectExists = s3Service->objectExists({bucketName, existingObjectKey1});

    EXPECT_TRUE(objectExists);
}
