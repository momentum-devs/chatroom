#pragma once

#include <string>

namespace server::application
{
struct ObjectExistsPayload
{
    std::string bucketName;
    std::string objectKey;
};

inline bool operator==(const ObjectExistsPayload& lhs, const ObjectExistsPayload& rhs)
{
    return (lhs.bucketName == rhs.bucketName && lhs.objectKey == rhs.objectKey);
}

}
