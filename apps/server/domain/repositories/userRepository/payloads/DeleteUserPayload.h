#pragma once

#include <string>

#include "../../../entities/user/User.h"

namespace server::domain
{
struct DeleteUserPayload
{
    User user;
};
}
