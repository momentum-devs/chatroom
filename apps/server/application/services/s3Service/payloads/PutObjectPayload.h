#pragma once

#include <string>

namespace server::application
{
struct PutObjectPayload
{
    std::string bucketName;
    std::string key;
    std::string data;
};

inline bool operator==(const PutObjectPayload& lhs, const PutObjectPayload& rhs)
{
    return (lhs.bucketName == rhs.bucketName && lhs.key == rhs.key && lhs.data == rhs.data);
}

}
