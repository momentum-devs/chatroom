#pragma once

#include <memory>
#include <vector>

#include "../../../domain/repositories/UserRepository.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class UserRepositoryImpl : public domain::UserRepository
{
public:
    explicit UserRepositoryImpl(std::unique_ptr<UserMapper>);

    domain::User createUser(const domain::CreateUserPayload&) const override;
    std::optional<domain::User> findUser(const domain::FindUserPayload&) const override;
    void deleteUser(const domain::DeleteUserPayload&) const override;

private:
    std::unique_ptr<UserMapper> userMapper;
};
}
