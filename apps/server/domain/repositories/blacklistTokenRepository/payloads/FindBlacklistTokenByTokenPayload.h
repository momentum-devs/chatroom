#pragma once

#include <string>

namespace server::domain
{
struct FindBlacklistTokenByTokenPayload
{
    const std::string token;
};

inline bool operator==(const FindBlacklistTokenByTokenPayload& lhs, const FindBlacklistTokenByTokenPayload& rhs)
{
    return lhs.token == rhs.token;
}
}
