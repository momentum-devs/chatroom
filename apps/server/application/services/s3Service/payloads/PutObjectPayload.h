#pragma once

#include <string>

namespace server::application
{
struct PutObjectPayload
{
    std::string bucketName;
    std::string objectKey;
    std::string data;
};

inline bool operator==(const PutObjectPayload& lhs, const PutObjectPayload& rhs)
{
    return (lhs.bucketName == rhs.bucketName && lhs.objectKey == rhs.objectKey && lhs.data == rhs.data);
}

}
