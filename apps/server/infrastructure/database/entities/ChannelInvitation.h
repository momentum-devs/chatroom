#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "Channel.h"
#include "User.h"

namespace server::infrastructure
{
#pragma db object table("channels_invitations")
class ChannelInvitation
{
public:
    ChannelInvitation(std::string idInit, std::shared_ptr<User> senderInit, std::shared_ptr<User> recipientInit,
                      std::shared_ptr<Channel> channelInit, std::string createdAtInit, std::string updatedAtInit)
        : id{std::move(idInit)},
          sender{std::move(senderInit)},
          recipient{std::move(recipientInit)},
          channel{std::move(channelInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)}
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

    std::shared_ptr<Channel> getChannel() const
    {
        return channel;
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
    ChannelInvitation() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
    std::string updated_at;

#pragma db not_null
    std::shared_ptr<User> sender;

#pragma db not_null
    std::shared_ptr<User> recipient;

#pragma db not_null
    std::shared_ptr<Channel> channel;
};
}
