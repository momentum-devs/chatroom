#pragma once

#include <string>

namespace server::domain
{
struct CreateBlacklistTokenPayload
{
    std::string id;
    std::string token;
};

inline bool operator==(const CreateBlacklistTokenPayload& lhs, const CreateBlacklistTokenPayload& rhs)
{
    auto tieStruct = [](const CreateBlacklistTokenPayload& payload) { return std::tie(payload.id, payload.token); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
