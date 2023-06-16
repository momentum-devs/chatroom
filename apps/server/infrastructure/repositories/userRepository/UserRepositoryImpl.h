#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "../../../domain/repositories/UserRepository.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class UserRepositoryImpl : public domain::UserRepository
{
public:
    UserRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::unique_ptr<UserMapper>);

    domain::User createUser(const domain::CreateUserPayload&) const override;
    std::optional<domain::User> findUserById(const domain::FindUserByIdPayload&) const override;
    std::optional<domain::User> findUserByEmail(const domain::FindUserByEmailPayload&) const override;
    void updateUser(const domain::UpdateUserPayload&) const override;
    void deleteUser(const domain::DeleteUserPayload&) const override;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::unique_ptr<UserMapper> userMapper;
};
}
