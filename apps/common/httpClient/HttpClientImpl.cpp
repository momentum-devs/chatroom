#include "HttpClientImpl.h"

#include <cpr/cpr.h>

namespace common::httpClient
{
HttpResponse HttpClientImpl::get(const HttpGetRequestPayload& payload) const
{
    auto queryParameters = cpr::Parameters{};

    if (payload.queries)
    {
        for (const auto& query : *payload.queries)
        {
            queryParameters.Add({query.first, query.second});
        }
    }

    cpr::Header headers;

    if (payload.headers)
    {
        for (const auto& header : *payload.headers)
        {
            headers.insert({header.first, header.second});
        }
    }

    const auto response = cpr::Get(cpr::Url{payload.url}, queryParameters, headers);

    return {static_cast<int>(response.status_code), response.text};
}

HttpResponse HttpClientImpl::post(const HttpPostRequestPayload& payload) const
{
    cpr::Header headers;

    if (payload.headers)
    {
        for (const auto& header : *payload.headers)
        {
            headers.insert({header.first, header.second});
        }
    }

    const auto response = cpr::Post(cpr::Url{payload.url}, cpr::Body{payload.body}, headers);

    return {static_cast<int>(response.status_code), response.text};
}
}
