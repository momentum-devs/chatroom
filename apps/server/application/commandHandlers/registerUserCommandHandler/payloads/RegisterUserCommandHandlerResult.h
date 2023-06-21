#pragma once

#include "../../../../domain/entities/user/User.h"

namespace server::application
{
struct RegisterUserCommandHandlerResult
{
    domain::User user;
};
}
