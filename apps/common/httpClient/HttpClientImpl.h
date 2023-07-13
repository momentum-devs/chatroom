#pragma once

#include "HttpClient.h"

namespace common::httpClient
{
class HttpClientImpl : public HttpClient
{
public:
    [[nodiscard]] HttpResponse get(const HttpGetRequestPayload&) const override;
};
}
