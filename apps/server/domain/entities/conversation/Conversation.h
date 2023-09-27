#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
class Conversation
{
public:
    Conversation(std::string id, std::shared_ptr<User> user, std::shared_ptr<User> recipient,
                 std::shared_ptr<Channel> channel, std::string createdAt, std::string updatedAt);

    std::string getId() const;
    std::shared_ptr<User> getUser() const;
    std::shared_ptr<User> getRecipient() const;
    std::shared_ptr<Channel> getChannel() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;

private:
    std::string id;
    std::string createdAt;
    std::string updatedAt;
    std::shared_ptr<User> user;
    std::shared_ptr<User> recipient;
    std::shared_ptr<Channel> channel;
};
}
