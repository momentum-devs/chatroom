#include "S3ServiceImpl.h"

#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/S3Client.h>

#include "loguru.hpp"

namespace server::application
{
S3ServiceImpl::S3ServiceImpl(std::unique_ptr<Aws::S3::S3Client> s3ClientInit) : s3Client{std::move(s3ClientInit)} {}

void S3ServiceImpl::putObject(const PutObjectPayload& payload) const
{
    Aws::S3::Model::PutObjectRequest request;

    request.SetBucket(payload.bucketName);
    request.SetKey(payload.objectKey);

    request.SetBody(std::make_shared<Aws::StringStream>(payload.data));

    auto result = s3Client->PutObject(request);

    if (result.IsSuccess())
    {
        LOG_S(INFO) << "Successfully uploaded file to S3.";
    }
    else
    {
        LOG_S(ERROR) << "Error uploading file to S3: " << result.GetError().GetMessage();

        throw std::runtime_error{result.GetError().GetMessage()};
    }
}

std::string S3ServiceImpl::getObject(const GetObjectPayload& payload) const
{
    Aws::S3::Model::GetObjectRequest request;

    request.SetBucket(payload.bucketName);
    request.SetKey(payload.objectKey);

    auto result = s3Client->GetObject(request);

    if (result.IsSuccess())
    {
        LOG_S(INFO) << "Successfully downloaded file from S3.";

        auto& retrievedFile = result.GetResultWithOwnership().GetBody();

        std::ostringstream contentStream;
        contentStream << retrievedFile.rdbuf();

        return contentStream.str();
    }
    else
    {
        LOG_S(ERROR) << "Error downloading file from S3: " << result.GetError().GetMessage();

        throw std::runtime_error{result.GetError().GetMessage()};
    }
}
}
