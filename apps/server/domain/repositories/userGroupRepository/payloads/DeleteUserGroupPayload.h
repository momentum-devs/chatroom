#pragma once

#include <string>

#include "../../../entities/userGroup/UserGroup.h"

namespace server::domain
{
struct DeleteUserGroupPayload
{
    UserGroup userGroup;
};
}
