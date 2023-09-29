#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "User.h"

namespace server::infrastructure
{
#pragma db object table("channels")
class Channel
{
public:
    Channel(std::string idInit, std::string nameInit, std::shared_ptr<User> creatorInit, std::string createdAtInit,
            std::string updatedAtInit)
        : id{std::move(idInit)},
          name{std::move(nameInit)},
          creator{std::move(creatorInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)}
    {
    }

    std::string getId() const
    {
        return id;
    }

    std::string getName() const
    {
        return name;
    }

    std::shared_ptr<User> getCreator() const
    {
        return creator;
    }

    std::string getCreatedAt() const
    {
        return created_at;
    }

    std::string getUpdatedAt() const
    {
        return updated_at;
    }

    bool operator==(const Channel& channel) const
    {
        auto tieStruct = [](const Channel& channel)
        { return std::tie(channel.id, channel.creator, channel.name, channel.created_at, channel.updated_at); };

        return tieStruct(*this) == tieStruct(channel);
    }

private:
    Channel() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string name;

#pragma db not_null
    std::shared_ptr<User> creator;

    std::string created_at;
    std::string updated_at;
};
}
