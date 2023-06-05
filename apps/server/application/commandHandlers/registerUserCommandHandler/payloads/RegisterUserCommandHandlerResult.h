#pragma once

#include <string>

#include "../../../../domain/entities/User.h"

namespace server::application
{
struct RegisterUserCommandHandlerResult
{
    domain::User user;
};
}
