#pragma once

#include <memory>

#include "HttpClient.h"

namespace common::httpClient
{
class HttpClientFactory
{
public:
    static std::unique_ptr<HttpClient> createHttpClient();
};
}
