#pragma once

#include <string>
#include <vector>

namespace server::domain
{
struct DeleteGroupsPayload
{
    std::vector<std::string> groupsIds;
};
}
