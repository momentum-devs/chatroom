#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>

#include "../../../domain/repositories/groupRepository/GroupRepository.h"
#include "groupMapper/GroupMapper.h"

namespace server::infrastructure
{
class GroupRepositoryImpl : public domain::GroupRepository
{
public:
    GroupRepositoryImpl(std::shared_ptr<odb::sqlite::database>, std::shared_ptr<GroupMapper>);

    std::shared_ptr<domain::Group> createGroup(const domain::CreateGroupPayload&) const override;
    std::optional<std::shared_ptr<domain::Group>> findGroupById(const domain::FindGroupByIdPayload&) const override;
    void deleteGroup(const domain::DeleteGroupPayload&) const override;
    void deleteGroups(const domain::DeleteGroupsPayload&) const override;

private:
    std::shared_ptr<odb::sqlite::database> db;
    std::shared_ptr<GroupMapper> groupMapper;
};
}
