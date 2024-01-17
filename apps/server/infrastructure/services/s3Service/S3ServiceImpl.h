#pragma once

#include <aws/s3/S3Client.h>
#include <memory>

#include "server/application/services/s3Service/S3Service.h"

namespace server::application
{
class S3ServiceImpl : public S3Service
{
public:
    explicit S3ServiceImpl(std::unique_ptr<Aws::S3::S3Client> s3Client);

    void putObject(const PutObjectPayload& payload) const;
    std::string getObject(const GetObjectPayload& payload) const;
    bool objectExists(const ObjectExistsPayload& payload) const;

private:
    std::unique_ptr<Aws::S3::S3Client> s3Client;
};
}
