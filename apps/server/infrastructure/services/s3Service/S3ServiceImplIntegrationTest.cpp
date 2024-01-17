#include "gtest/gtest.h"

#include "httpClient/HttpClientMock.h"

#include "faker-cxx/Internet.h"
#include "faker-cxx/Lorem.h"
#include "faker-cxx/Word.h"
#include "fmt/format.h"
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
    const std::string s3Endpoint = "http://127.0.0.1:4566";

    const std::shared_ptr<S3Service> s3Service =
        S3ServiceFactory::create({awsAccountId, awsAccountSecretKey, s3Endpoint});
};

TEST_F(S3ServiceImplIntegrationTest, shouldGetObject)
{
    const auto bucketName = faker::Internet::domainName();
    const auto objectKey = faker::Internet::domainName();
    const auto objectData = faker::Lorem::paragraph();

    const auto s3Client = Aws::S3::S3Client(Aws::Auth::AWSCredentials(awsAccountId, awsAccountSecretKey),
                                            Aws::Client::ClientConfiguration().WithEndpointOverride(s3Endpoint));

    Aws::S3::Model::CreateBucketRequest createBucketRequest;
    createBucketRequest.SetBucket(bucketName.c_str());
    s3Client.CreateBucket(createBucketRequest);

    Aws::S3::Model::PutObjectRequest putObjectRequest;
    putObjectRequest.SetBucket(bucketName.c_str());
    putObjectRequest.SetKey(objectKey.c_str());
    putObjectRequest.SetBody(objectData.c_str());
    s3Client.PutObject(putObjectRequest);

    const auto getObjectPayload = GetObjectPayload{bucketName, objectKey};

    const auto result = s3Service->getObject(getObjectPayload);

    EXPECT_EQ(result, objectData);
}

TEST_F(S3ServiceImplIntegrationTest, shouldPutObject)
{
    const auto bucketName = faker::Internet::domainName();
    const auto objectKey = faker::Internet::domainName();
    const auto objectData = faker::Lorem::paragraph();

    const auto putObjectPayload = PutObjectPayload{bucketName, objectKey, objectData};

    s3Service->putObject(putObjectPayload);

    const auto s3Client = Aws::S3::S3Client(Aws::Auth::AWSCredentials(awsAccountId, awsAccountSecretKey),
                                            Aws::Client::ClientConfiguration().WithEndpointOverride(s3Endpoint));

    Aws::S3::Model::GetObjectRequest getObjectRequest;
    getObjectRequest.SetBucket(bucketName.c_str());
    getObjectRequest.SetKey(objectKey.c_str());
    const auto result = s3Client.GetObject(getObjectRequest);

    EXPECT_EQ(result.IsSuccess(), true);
}
