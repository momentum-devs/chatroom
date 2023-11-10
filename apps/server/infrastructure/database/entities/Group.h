#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "User.h"

namespace server::infrastructure
{
#pragma db object table("groups")
class Group
{
public:
    Group(std::string idInit, std::string createdAtInit) : id{std::move(idInit)}, created_at{std::move(createdAtInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

    bool operator==(const Group& group) const
    {
        auto tieStruct = [](const Group& group) { return std::tie(group.id, group.created_at); };

        return tieStruct(*this) == tieStruct(group);
    }

private:
    Group() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
};
}
