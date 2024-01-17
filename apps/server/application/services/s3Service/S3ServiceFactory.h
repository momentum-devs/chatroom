#pragma once

#include <memory>
#include <optional>

#include "S3Service.h"

namespace server::application
{
struct AwsConfig
{
    std::string accessKeyId;
    std::string secretAccessKey;
    std::optional<std::string> endpoint;
};

class S3ServiceFactory
{
public:
    static std::shared_ptr<S3Service> create(const AwsConfig& awsConfig);
};
}
