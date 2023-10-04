#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "Channel.h"
#include "User.h"

namespace server::infrastructure
{
#pragma db object table("conversations")
class Conversation
{
public:
    Conversation(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<User> recipientInit,
                 std::shared_ptr<Channel> channelInit, std::string createdAtInit, std::string updatedAtInit)
        : id{std::move(idInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)},
          user{std::move(userInit)},
          recipient{std::move(recipientInit)},
          channel{std::move(channelInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::shared_ptr<User> getUser() const
    {
        return user;
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

    [[nodiscard]] std::string getUpdatedAt() const
    {
        return updated_at;
    }

private:
    Conversation() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
    std::string updated_at;
    std::shared_ptr<User> user;
    std::shared_ptr<User> recipient;
    std::shared_ptr<Channel> channel;
};
}
