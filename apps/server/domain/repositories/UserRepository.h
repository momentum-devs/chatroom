#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../entities/User.h"
#include "payloads/CreateUserPayload.h"
#include "payloads/DeleteUserPayload.h"
#include "payloads/UpdateUserPayload.h"
#include "server/domain/repositories/payloads/FindUserByEmailPayload.h"
#include "server/domain/repositories/payloads/FindUserByIdPayload.h"

namespace server::domain
{
class UserRepository
{
public:
    virtual ~UserRepository() = default;

    virtual User createUser(const CreateUserPayload&) const = 0;
    virtual std::optional<User> findUserById(const FindUserByIdPayload&) const = 0;
    virtual std::optional<User> findUserByEmail(const FindUserByEmailPayload&) const = 0;
    virtual void updateUser(const UpdateUserPayload&) const = 0;
    virtual void deleteUser(const DeleteUserPayload&) const = 0;
};
}
