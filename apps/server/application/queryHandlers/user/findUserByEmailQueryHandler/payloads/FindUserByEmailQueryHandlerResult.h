#pragma once

#include <vector>

#include "../../../../../domain/entities/user/User.h"

namespace server::application
{
struct FindUserByEmailQueryHandlerResult
{
    domain::User user;
};
}
