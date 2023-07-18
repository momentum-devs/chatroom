#pragma once

#include "HttpResponse.h"
#include "payloads/HttpGetRequestPayload.h"
#include "payloads/HttpPostRequestPayload.h"

namespace common::httpClient
{
class HttpClient
{
public:
    virtual ~HttpClient() = default;

    virtual HttpResponse get(const HttpGetRequestPayload&) const = 0;
    virtual HttpResponse post(const HttpPostRequestPayload&) const = 0;
};
}
