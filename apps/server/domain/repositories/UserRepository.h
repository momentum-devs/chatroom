#pragma once

#include <string>
#include <vector>
#include <optional>

#include "payloads/CreateUserPayload.h"
#include "payloads/FindUserPayload.h"
#include "payloads/DeleteUserPayload.h"
#include "../entities/User.h"

namespace server::domain
{
class UserRepository
{
public:
    virtual ~UserRepository() = default;

    virtual User createUser(const CreateUserPayload&) const = 0;
    virtual std::optional<User> findUser(const FindUserPayload&) const = 0;
    virtual void deleteUser(const DeleteUserPayload&) const = 0;
};
}
