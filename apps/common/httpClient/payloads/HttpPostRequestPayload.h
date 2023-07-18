#pragma once

#include <map>
#include <optional>
#include <string>

namespace common::httpClient
{
struct HttpPostRequestPayload
{
    std::string url;
    std::optional<std::map<std::string, std::string>> headers = std::nullopt;
    std::string body;
};

inline bool operator==(const HttpPostRequestPayload& lhs, const HttpPostRequestPayload& rhs)
{
    return (lhs.url == rhs.url && lhs.headers == rhs.headers && lhs.body == rhs.body);
}

}
