#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>
#include <vector>

#include "../../../domain/repositories/userRepository/UserRepository.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class UserRepositoryImpl : public domain::UserRepository
{
public:
    UserRepositoryImpl(std::shared_ptr<odb::sqlite::database>, std::shared_ptr<UserMapper>);

    std::shared_ptr<domain::User> createUser(const domain::CreateUserPayload&) const override;
    std::optional<std::shared_ptr<domain::User>> findUserById(const domain::FindUserByIdPayload&) const override;
    std::optional<std::shared_ptr<domain::User>> findUserByEmail(const domain::FindUserByEmailPayload&) const override;
    std::shared_ptr<domain::User> updateUser(const domain::UpdateUserPayload&) const override;
    void deleteUser(const domain::DeleteUserPayload&) const override;

private:
    std::shared_ptr<odb::sqlite::database> db;
    std::shared_ptr<UserMapper> userMapper;
};
}
