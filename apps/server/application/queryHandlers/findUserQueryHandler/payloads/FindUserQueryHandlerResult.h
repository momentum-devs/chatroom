#pragma once

#include <vector>

#include "../../../../domain/entities/user/User.h"

namespace server::application
{
struct FindUserQueryHandlerResult
{
    domain::User user;
};
}
