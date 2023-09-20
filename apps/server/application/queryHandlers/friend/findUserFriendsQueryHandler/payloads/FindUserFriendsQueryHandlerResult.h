#pragma once

#include <vector>

#include "../../../../../domain/entities/user/User.h"

namespace server::application
{
struct FindUserFriendsQueryHandlerResult
{
    std::vector<domain::User> friends;
};
}
