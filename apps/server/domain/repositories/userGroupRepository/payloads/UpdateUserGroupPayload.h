#pragma once

#include <optional>
#include <string>

#include "../../../entities/userGroup/UserGroup.h"

namespace server::domain
{
struct UpdateUserGroupPayload
{
    UserGroup userGroup;
};
}
