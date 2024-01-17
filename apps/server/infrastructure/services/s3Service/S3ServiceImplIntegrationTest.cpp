#include "gtest/gtest.h"

#include "faker-cxx/Internet.h"
#include "faker-cxx/Lorem.h"
#include "S3ServiceImpl.h"
#include "server/application/services/s3Service/S3ServiceFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::application;

class S3ServiceImplIntegrationTest : public Test
{
public:
    const std::string awsAccountId = "test";
    const std::string awsAccountSecretKey = "test";
    const std::string bucketName = "avatars";
    const std::string existingObjectKey = "example_avatar.jpg";
    const std::string nonExistingObjectKey = "another_avatar.jpg";
    const std::string s3Endpoint = "http://127.0.0.1:4566";

    const std::shared_ptr<S3Service> s3Service =
        S3ServiceFactory::create({awsAccountId, awsAccountSecretKey, s3Endpoint});
};

TEST_F(S3ServiceImplIntegrationTest, shouldGetObject)
{
    const auto object = s3Service->getObject({bucketName, existingObjectKey});

    EXPECT_FALSE(object.empty());
}

TEST_F(S3ServiceImplIntegrationTest, givenNonExistingObjectKey_shouldThrow)
{
    EXPECT_THROW(s3Service->getObject({bucketName, nonExistingObjectKey}), std::runtime_error);
}

TEST_F(S3ServiceImplIntegrationTest, shouldPutObject)
{
    const auto objectKey = faker::Internet::password();
    const auto objectData = faker::Lorem::paragraph();

    s3Service->putObject({bucketName, objectKey, objectData});

    const auto object = s3Service->getObject({bucketName, objectKey});

    EXPECT_EQ(object, objectData);
}
