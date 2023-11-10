#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../../entities/group/Group.h"
#include "payloads/CreateGroupPayload.h"
#include "payloads/DeleteGroupPayload.h"
#include "payloads/FindGroupByIdPayload.h"

namespace server::domain
{
class GroupRepository
{
public:
    virtual ~GroupRepository() = default;

    virtual std::shared_ptr<Group> createGroup(const CreateGroupPayload&) const = 0;
    virtual std::optional<std::shared_ptr<Group>> findGroupById(const FindGroupByIdPayload&) const = 0;
    virtual void deleteGroup(const DeleteGroupPayload&) const = 0;
};
}
