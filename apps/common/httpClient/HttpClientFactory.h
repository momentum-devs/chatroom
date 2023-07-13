#pragma once

#include <memory>

#include "HttpClient.h"

namespace common::httpClient
{
class HttpClientFactory
{
public:
    std::unique_ptr<HttpClient> createHttpClient() const;
};
}
