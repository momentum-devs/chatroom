#pragma once

#include <memory>
#include <optional>
#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
class UserChannel
{
public:
    UserChannel(std::string id, std::optional<std::string> lastReadMessageId, std::shared_ptr<User> user,
                std::shared_ptr<Channel> channel, std::string createdAt);

    std::string getId() const;
    std::optional<std::string> getLastReadMessageId() const;
    std::shared_ptr<User> getUser() const;
    std::shared_ptr<Channel> getChannel() const;
    std::string getCreatedAt() const;

private:
    std::string id;
    std::string createdAt;
    std::optional<std::string> lastReadMessageId;
    std::shared_ptr<User> user;
    std::shared_ptr<Channel> channel;
};
}
