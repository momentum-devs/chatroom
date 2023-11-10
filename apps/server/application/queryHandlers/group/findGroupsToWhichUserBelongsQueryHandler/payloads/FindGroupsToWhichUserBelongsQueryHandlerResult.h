#pragma once

#include <vector>

#include "../../../../../domain/entities/group/Group.h"

namespace server::application
{
struct FindGroupsToWhichUserBelongsQueryHandlerResult
{
    std::vector<domain::Group> groups;
};
}
