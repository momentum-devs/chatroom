#include "S3ServiceImpl.h"

#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/S3Client.h>

namespace server::application
{

S3ServiceImpl::S3ServiceImpl(std::unique_ptr<Aws::S3::S3Client> s3ClientInit) : s3Client{std::move(s3ClientInit)}
{
    Aws::Auth::AWSCredentials credentials("your-access-key-id", "your-secret-access-key");

    s3Client = std::make_unique<Aws::S3::S3Client>(credentials);
}

void S3ServiceImpl::putObject(const PutObjectPayload&) const {}

std::string S3ServiceImpl::getObject(const GetObjectPayload&) const
{
    return "";
}
}
