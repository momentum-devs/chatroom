#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/user/User.h"

namespace server::domain
{
class Channel
{
public:
    Channel(std::string id, std::string name, std::shared_ptr<User> creator, std::string createdAt,
            std::string updatedAt, const std::optional<std::string>& avatarUrl);

    std::string getId() const;
    std::string getName() const;
    std::shared_ptr<User> getCreator() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;
    std::optional<std::string> getAvatarUrl() const;
    void setName(const std::string&);
    void setAvatarUrl(const std::string& avatarUrl);
    bool operator==(const Channel&) const;

private:
    std::string id;
    std::string name;
    std::shared_ptr<User> creator;
    std::string createdAt;
    std::string updatedAt;
    std::optional<std::string> avatarUrl;
};
}
