#include "Group.h"

#include <utility>

namespace server::domain
{
Group::Group(std::string idInit, std::string createdAtInit) : id{std::move(idInit)}, createdAt{std::move(createdAtInit)}
{
}

std::string Group::getId() const
{
    return id;
}

std::string Group::getCreatedAt() const
{
    return createdAt;
}
bool Group::operator==(const Group& group) const
{
    auto tieStruct = [](const Group& group) { return std::tie(group.id, group.createdAt); };

    return tieStruct(*this) == tieStruct(group);
}
}
