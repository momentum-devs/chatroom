#pragma once

#include <string>

namespace server::domain
{
class UserChannel
{
public:
    UserChannel(std::string id, std::string userId, std::string channelId, std::string createdAt,
                std::string updatedAt);

    std::string getId() const;
    std::string getUserId() const;
    std::string getChannelId() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;

private:
    std::string id;
    std::string userId;
    std::string channelId;
    std::string createdAt;
    std::string updatedAt;
};
}
