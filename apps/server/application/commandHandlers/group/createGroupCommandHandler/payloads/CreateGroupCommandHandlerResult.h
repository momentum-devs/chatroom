#pragma once

#include "../../../../../domain/entities/group/Group.h"

namespace server::application
{
struct CreateGroupCommandHandlerResult
{
    domain::Group group;
};
}
