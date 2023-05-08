#pragma once

#include <string>
#include <vector>

#include "../domain/User.h"

namespace server::application
{
class UserRepository
{
public:
    virtual ~UserRepository() = default;

    virtual std::vector<domain::User> findUsers() = 0;
    virtual void deleteUser(const std::string& id) = 0;
};
}
