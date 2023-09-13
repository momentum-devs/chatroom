#pragma once

#include <vector>

#include "../../../../../domain/entities/user/User.h"

namespace server::application
{
struct FindUsersBelongingToChannelQueryHandlerResult
{
    std::vector<domain::User> users;
};
}
