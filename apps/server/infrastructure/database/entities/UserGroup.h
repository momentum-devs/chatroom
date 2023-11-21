#pragma once

#include <memory>
#include <odb/core.hxx>
#include <odb/nullable.hxx>
#include <string>
#include <utility>

#include "Group.h"
#include "User.h"

namespace server::infrastructure
{
#pragma db object table("users_groups")
class UserGroup
{
public:
    UserGroup(std::string idInit, const odb::nullable<std::string>& lastReadMessageIdInit,
              std::shared_ptr<User> userInit, std::shared_ptr<Group> groupInit, std::string createdAtInit)
        : id{std::move(idInit)},
          created_at{std::move(createdAtInit)},
          last_read_message_id{lastReadMessageIdInit},
          user{std::move(userInit)},
          group{std::move(groupInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] odb::nullable<std::string> getLastReadMessageId() const
    {
        return last_read_message_id;
    }

    [[nodiscard]] std::shared_ptr<User> getUser() const
    {
        return user;
    }

    [[nodiscard]] std::shared_ptr<Group> getGroup() const
    {
        return group;
    }

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

    void setLastReadMessageId(const std::string& lastReadMessageId)
    {
        last_read_message_id = lastReadMessageId;
    }

private:
    UserGroup() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
    odb::nullable<std::string> last_read_message_id;

#pragma db not_null
    std::shared_ptr<User> user;

#pragma db not_null
    std::shared_ptr<Group> group;
};
}
