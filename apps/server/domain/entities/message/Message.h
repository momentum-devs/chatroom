#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
class Message
{
public:
    Message(std::string id, std::string content, std::shared_ptr<User> sender, std::shared_ptr<Channel> channel,
            std::shared_ptr<Group> group, std::string createdAt, std::string updatedAt);

    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getContent() const;
    [[nodiscard]] std::shared_ptr<User> getSender() const;
    [[nodiscard]] std::shared_ptr<Channel> getChannel() const;
    [[nodiscard]] std::shared_ptr<Group> getGroup() const;
    [[nodiscard]] std::string getCreatedAt() const;
    [[nodiscard]] std::string getUpdatedAt() const;
    void setContent(const std::string&);

private:
    std::string id;
    std::string createdAt;
    std::string updatedAt;
    std::string content;
    std::shared_ptr<User> sender;
    std::shared_ptr<Channel> channel;
    std::shared_ptr<Group> group;
};
}
