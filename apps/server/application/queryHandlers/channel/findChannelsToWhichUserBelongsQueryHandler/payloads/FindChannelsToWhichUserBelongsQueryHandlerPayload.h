#pragma once

#include <string>

namespace server::application
{
struct FindChannelsToWhichUserBelongsQueryHandlerPayload
{
    const std::string userId;
};

inline bool operator==(const FindChannelsToWhichUserBelongsQueryHandlerPayload& lhs,
                       const FindChannelsToWhichUserBelongsQueryHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId;
}
}
