#pragma once

#include <string>

#include "../../entities/User.h"

namespace server::domain
{
struct UpdateUserPayload
{
    User user;
};
}
