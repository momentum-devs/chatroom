#pragma once

#include "gmock/gmock.h"

#include "HttpClient.h"

namespace common::httpClient
{
class HttpClientMock : public HttpClient
{
public:
    MOCK_METHOD(HttpResponse, get, (const HttpGetRequestPayload&), (const override));
    MOCK_METHOD(HttpResponse, post, (const HttpPostRequestPayload&), (const override));
};
}
