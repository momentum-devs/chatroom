#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "User.h"

namespace server::infrastructure
{
#pragma db object table("friends_invitations")
class FriendInvitation
{
public:
    FriendInvitation(std::string idInit, std::shared_ptr<User> senderInit, std::shared_ptr<User> recipientInit,
                     std::string createdAtInit, std::string updatedAtInit)
        : id{std::move(idInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)},
          sender{std::move(senderInit)},
          recipient{std::move(recipientInit)}

    {
    }

    std::string getId() const
    {
        return id;
    }

    std::shared_ptr<User> getSender() const
    {
        return sender;
    }

    std::shared_ptr<User> getRecipient() const
    {
        return recipient;
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
    FriendInvitation() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
    std::string updated_at;

#pragma db not_null
    std::shared_ptr<User> sender;

#pragma db not_null
    std::shared_ptr<User> recipient;
};
}
