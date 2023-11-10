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
                      std::shared_ptr<Channel> channelInit, std::string createdAtInit)
        : id{std::move(idInit)},
          created_at{std::move(createdAtInit)},
          sender{std::move(senderInit)},
          recipient{std::move(recipientInit)},
          channel{std::move(channelInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::shared_ptr<User> getSender() const
    {
        return sender;
    }

    [[nodiscard]] std::shared_ptr<User> getRecipient() const
    {
        return recipient;
    }

    [[nodiscard]] std::shared_ptr<Channel> getChannel() const
    {
        return channel;
    }

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

private:
    ChannelInvitation() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;

#pragma db not_null
    std::shared_ptr<User> sender;

#pragma db not_null
    std::shared_ptr<User> recipient;

#pragma db not_null
    std::shared_ptr<Channel> channel;
};
}
