#pragma once

#include "../../../../../domain/entities/user/User.h"

namespace server::application
{
struct UpdateUserCommandHandlerResult
{
    domain::User user;
};
}
