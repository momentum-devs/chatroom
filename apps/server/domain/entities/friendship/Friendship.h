#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
class Friendship
{
public:
    Friendship(std::string id, std::shared_ptr<User> user, std::shared_ptr<User> userFriend, std::string createdAt,
               std::shared_ptr<Group> group);

    std::string getId() const;
    std::shared_ptr<User> getUser() const;
    std::shared_ptr<User> getUserFriend() const;
    std::string getCreatedAt() const;
    std::shared_ptr<Group> getGroup() const;

private:
    std::string id;
    std::string createdAt;
    std::shared_ptr<User> user;
    std::shared_ptr<User> userFriend;
    std::shared_ptr<Group> group;
};
}
