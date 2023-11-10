#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateGroupPayload
{
    const std::string id;
};

inline bool operator==(const CreateGroupPayload& lhs, const CreateGroupPayload& rhs)
{
    auto tieStruct = [](const CreateGroupPayload& payload) { return std::tie(payload.id); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
