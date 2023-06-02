#pragma once

#include <string>
#include "../../entities/User.h"

namespace server::domain
{
struct DeleteUserPayload
{
    User user;
};
}
