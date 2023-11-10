#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
class ChannelInvitation
{
public:
    ChannelInvitation(std::string id, std::shared_ptr<User> sender, std::shared_ptr<User> recipient,
                      std::shared_ptr<Channel> channel, std::string createdAt);

    std::string getId() const;
    std::shared_ptr<User> getSender() const;
    std::shared_ptr<User> getRecipient() const;
    std::shared_ptr<Channel> getChannel() const;
    std::string getCreatedAt() const;

private:
    std::string id;
    std::string createdAt;
    std::shared_ptr<User> sender;
    std::shared_ptr<User> recipient;
    std::shared_ptr<Channel> channel;
};
}
