#pragma once

#include <memory>
#include <optional>
#include <string>

#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
class UserGroup
{
public:
    UserGroup(std::string id, std::optional<std::string> lastReadMessageId, std::shared_ptr<User> user,
              std::shared_ptr<Group> group, std::string createdAt);

    std::string getId() const;
    std::optional<std::string> getLastReadMessageId() const;
    std::shared_ptr<User> getUser() const;
    std::shared_ptr<Group> getGroup() const;
    std::string getCreatedAt() const;
    void setLastReadMessageId(const std::string& lastReadMessageId);

private:
    std::string id;
    std::string createdAt;
    std::optional<std::string> lastReadMessageId;
    std::shared_ptr<User> user;
    std::shared_ptr<Group> group;
};
}
