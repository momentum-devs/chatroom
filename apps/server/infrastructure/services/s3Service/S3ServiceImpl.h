#pragma once

#include <memory>

#include "server/application/services/s3Service/S3Service.h"

namespace server::application
{
class S3ServiceImpl : public S3Service
{
public:
    S3ServiceImpl();

    void putObject(const PutObjectPayload& payload) const;
    std::string getObject(const GetObjectPayload& payload) const;
};
}
