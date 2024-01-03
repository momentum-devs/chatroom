#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "Channel.h"
#include "Group.h"
#include "User.h"

namespace server::infrastructure
{
#pragma db object table("messages")
class Message
{
public:
    Message(std::string idInit, std::string contentInit, std::shared_ptr<User> senderInit,
            std::shared_ptr<Channel> channelInit, std::shared_ptr<Group> groupInit, std::string createdAtInit,
            std::string updatedAtInit)
        : id{std::move(idInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)},
          content{std::move(contentInit)},
          sender{std::move(senderInit)},
          channel{std::move(channelInit)},
          group{std::move(groupInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

    [[nodiscard]] std::string getUpdatedAt() const
    {
        return updated_at;
    }

    [[nodiscard]] std::string getContent() const
    {
        return content;
    }

    [[nodiscard]] std::shared_ptr<User> getSender() const
    {
        return sender;
    }

    [[nodiscard]] std::shared_ptr<Channel> getChannel() const
    {
        return channel;
    }

    [[nodiscard]] std::shared_ptr<Group> getGroup() const
    {
        return group;
    }

    void setContent(const std::string& newContent)
    {
        content = newContent;
    }

    void setUpdatedAt(const std::string& updatedAtInit)
    {
        updated_at = updatedAtInit;
    }

private:
    Message() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
    std::string updated_at;
    std::string content;

#pragma db not_null
    std::shared_ptr<User> sender;

    std::shared_ptr<Channel> channel;
    std::shared_ptr<Group> group;
};
}
