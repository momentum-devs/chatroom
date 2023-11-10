#pragma once

#include <string>

#include "../../../entities/group/Group.h"

namespace server::domain
{
struct DeleteGroupPayload
{
    Group group;
};
}
