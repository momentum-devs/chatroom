#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/user/User.h"

namespace server::domain
{
class FriendInvitation
{
public:
    FriendInvitation(std::string id, std::shared_ptr<User> sender, std::shared_ptr<User> recipient,
                     std::string createdAt, std::string updatedAt);

    std::string getId() const;
    std::shared_ptr<User> getSender() const;
    std::shared_ptr<User> getRecipient() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;

private:
    std::string id;
    std::string createdAt;
    std::string updatedAt;
    std::shared_ptr<User> sender;
    std::shared_ptr<User> recipient;
};
}
