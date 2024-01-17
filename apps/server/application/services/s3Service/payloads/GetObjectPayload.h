#pragma once

#include <string>

namespace server::application
{
struct GetObjectPayload
{
    std::string bucketName;
    std::string objectKey;
};

inline bool operator==(const GetObjectPayload& lhs, const GetObjectPayload& rhs)
{
    return (lhs.bucketName == rhs.bucketName && lhs.objectKey == rhs.objectKey);
}

}
