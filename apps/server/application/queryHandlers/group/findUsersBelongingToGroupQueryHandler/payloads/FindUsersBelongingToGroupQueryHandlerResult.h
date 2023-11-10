#pragma once

#include <vector>

#include "../../../../../domain/entities/user/User.h"

namespace server::application
{
struct FindUsersBelongingToGroupQueryHandlerResult
{
    std::vector<domain::User> users;
};
}
