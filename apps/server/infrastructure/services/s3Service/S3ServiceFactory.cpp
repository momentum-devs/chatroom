#include "server/application/services/s3Service/S3ServiceFactory.h"

#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/S3Client.h>

#include "S3ServiceImpl.h"

namespace server::application
{
std::shared_ptr<S3Service> S3ServiceFactory::create(const AwsConfig& awsConfig)
{
    Aws::Auth::AWSCredentials credentials(awsConfig.accessKeyId, awsConfig.secretAccessKey);

    Aws::Client::ClientConfiguration clientConfig;

    if (awsConfig.endpoint)
    {
        clientConfig.endpointOverride = *awsConfig.endpoint;
    }

    return std::make_shared<S3ServiceImpl>(std::make_unique<Aws::S3::S3Client>(
        credentials, Aws::MakeShared<Aws::S3::S3EndpointProvider>(Aws::S3::S3Client::ALLOCATION_TAG), clientConfig));
}

}
