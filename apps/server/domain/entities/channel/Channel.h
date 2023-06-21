#pragma once

#include <string>

namespace server::domain
{
class Channel
{
public:
    Channel(std::string id, std::string name, std::string creatorId, std::string createdAt, std::string updatedAt);

    std::string getId() const;
    std::string getName() const;
    std::string getCreatorId() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;

private:
    std::string id;
    std::string name;
    std::string creatorId;
    std::string createdAt;
    std::string updatedAt;
};
}
