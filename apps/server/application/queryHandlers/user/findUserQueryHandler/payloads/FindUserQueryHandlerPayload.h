#pragma once

#include <string>

namespace server::application
{
struct FindUserQueryHandlerPayload
{
    const std::string id;
};

inline bool operator==(const FindUserQueryHandlerPayload& lhs, const FindUserQueryHandlerPayload& rhs)
{
    return lhs.id == rhs.id;
}
}
