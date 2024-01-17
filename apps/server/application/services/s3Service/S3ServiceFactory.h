#pragma once

#include <memory>

#include "S3Service.h"

namespace server::application
{
struct AwsConfig
{
    std::string accessKeyId;
    std::string secretAccessKey;
};

class S3ServiceFactory
{
public:
    static std::shared_ptr<S3Service> create(const AwsConfig& awsConfig);
};
}
