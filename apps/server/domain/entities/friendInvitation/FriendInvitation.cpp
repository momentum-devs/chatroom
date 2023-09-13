#include "FriendInvitation.h"

#include <utility>

namespace server::domain
{
FriendInvitation::FriendInvitation(std::string idInit, std::shared_ptr<User> senderInit,
                                   std::shared_ptr<User> recipientInit, std::string createdAtInit,
                                   std::string updatedAtInit)
    : id{std::move(idInit)},
      sender{std::move(senderInit)},
      recipient{std::move(recipientInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)}
{
}

std::string FriendInvitation::getId() const
{
    return id;
}

std::shared_ptr<User> FriendInvitation::getSender() const
{
    return sender;
}

std::shared_ptr<User> FriendInvitation::getRecipient() const
{
    return recipient;
}

std::string FriendInvitation::getCreatedAt() const
{
    return createdAt;
}

std::string FriendInvitation::getUpdatedAt() const
{
    return updatedAt;
}

}
