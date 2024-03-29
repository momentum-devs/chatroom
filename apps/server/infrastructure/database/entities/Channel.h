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
            std::string updatedAtInit, const odb::nullable<std::string>& avatarUrlInit)
        : id{std::move(idInit)},
          name{std::move(nameInit)},
          creator{std::move(creatorInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)},
          avatar_url{avatarUrlInit}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::string getName() const
    {
        return name;
    }

    [[nodiscard]] std::shared_ptr<User> getCreator() const
    {
        return creator;
    }

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

    [[nodiscard]] std::string getUpdatedAt() const
    {
        return updated_at;
    }

    [[nodiscard]] odb::nullable<std::string> getAvatarUrl() const
    {
        return avatar_url;
    }

    void setName(const std::string& newName)
    {
        name = newName;
    }

    void setAvatarUrl(const std::string& newAvatarUrl)
    {
        avatar_url = newAvatarUrl;
    }

    void setUpdatedAt(const std::string& updatedAtInit)
    {
        updated_at = updatedAtInit;
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
    odb::nullable<std::string> avatar_url;
};
}
