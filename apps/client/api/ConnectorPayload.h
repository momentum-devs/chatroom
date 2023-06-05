#pragma once

#include <string>
#include <tuple>

namespace client::api
{
struct ConnectorPayload
{
    std::string host;
    unsigned short port;
};

inline bool operator==(const ConnectorPayload& lhs, const ConnectorPayload& rhs)
{
    auto tieStruct = [](const ConnectorPayload& connectorPayload)
    { return std::tie(connectorPayload.host, connectorPayload.port); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}