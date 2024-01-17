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

    return std::make_shared<S3ServiceImpl>(std::make_unique<Aws::S3::S3Client>(credentials));
}

}
