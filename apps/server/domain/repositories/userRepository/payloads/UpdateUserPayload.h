#pragma once

#include <string>

#include "../../../entities/user/User.h"

namespace server::domain
{
struct UpdateUserPayload
{
    User user;
};
}
