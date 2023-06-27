#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

namespace server::infrastructure
{
#pragma db value(std::string) type("TEXT") id_type("VARCHAR(32)")

#pragma db object table("users_channels")
class UserChannel
{
public:
    UserChannel(std::string idInit, std::string userIdInit, std::string channelIdInit, std::string createdAtInit,
                std::string updatedAtInit)
        : id{std::move(idInit)},
          user_id{std::move(userIdInit)},
          channel_id{std::move(channelIdInit)},
          created_at{createdAtInit},
          updated_at{updatedAtInit}
    {
    }

    std::string getId() const
    {
        return id;
    }

    std::string getUserId() const
    {
        return user_id;
    }

    std::string getChannelId() const
    {
        return channel_id;
    }

    std::string getCreatedAt() const
    {
        return created_at;
    }

    std::string getUpdatedAt() const
    {
        return updated_at;
    }

private:
    UserChannel() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string user_id;
    std::string channel_id;
    std::string created_at;
    std::string updated_at;
};
}
