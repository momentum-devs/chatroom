#pragma once

#include <string>

namespace server::application
{
struct GetObjectPayload
{
    std::string bucketName;
    std::string key;
    std::string data;
};

inline bool operator==(const GetObjectPayload& lhs, const GetObjectPayload& rhs)
{
    return (lhs.bucketName == rhs.bucketName && lhs.key == rhs.key && lhs.data == rhs.data);
}

}
