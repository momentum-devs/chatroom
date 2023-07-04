#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../../entities/user/User.h"
#include "payloads/CreateUserPayload.h"
#include "payloads/DeleteUserPayload.h"
#include "payloads/UpdateUserPayload.h"
#include "server/domain/repositories/userRepository/payloads/FindUserByEmailPayload.h"
#include "server/domain/repositories/userRepository/payloads/FindUserByIdPayload.h"

namespace server::domain
{
class UserRepository
{
public:
    virtual ~UserRepository() = default;

    virtual std::shared_ptr<User> createUser(const CreateUserPayload&) const = 0;
    virtual std::optional<std::shared_ptr<User>> findUserById(const FindUserByIdPayload&) const = 0;
    virtual std::optional<std::shared_ptr<User>> findUserByEmail(const FindUserByEmailPayload&) const = 0;
    virtual std::shared_ptr<domain::User> updateUser(const UpdateUserPayload&) const = 0;
    virtual void deleteUser(const DeleteUserPayload&) const = 0;
};
}
