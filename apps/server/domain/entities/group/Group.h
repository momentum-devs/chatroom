#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/user/User.h"

namespace server::domain
{
class Group
{
public:
    Group(std::string id, std::string createdAt);

    std::string getId() const;
    std::string getCreatedAt() const;

    bool operator==(const Group&) const;

private:
    std::string id;
    std::string createdAt;
};
}
