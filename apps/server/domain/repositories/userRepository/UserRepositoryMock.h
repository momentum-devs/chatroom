#pragma once

#include <gmock/gmock.h>

#include "UserRepository.h"

namespace server::domain
{
class UserRepositoryMock : public UserRepository
{
public:
    MOCK_METHOD(std::shared_ptr<User>, createUser, (const CreateUserPayload&), (const override));
    MOCK_METHOD(std::optional<std::shared_ptr<User>>, findUserById, (const FindUserByIdPayload&), (const override));
    MOCK_METHOD(std::optional<std::shared_ptr<User>>, findUserByEmail, (const FindUserByEmailPayload&),
                (const override));
    MOCK_METHOD(std::shared_ptr<domain::User>, updateUser, (const UpdateUserPayload&), (const override));
    MOCK_METHOD(void, deleteUser, (const DeleteUserPayload&), (const override));
};
}
