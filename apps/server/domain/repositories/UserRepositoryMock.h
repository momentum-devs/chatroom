#pragma once

#include <gmock/gmock.h>

#include "UserRepository.h"

namespace server::domain
{
class UserRepositoryMock : public UserRepository
{
public:
    MOCK_METHOD(User, createUser, (const CreateUserPayload&), (const override));
    MOCK_METHOD(std::optional<User>, findUserById, (const FindUserByIdPayload&), (const override));
    MOCK_METHOD(std::optional<User>, findUserByEmail, (const FindUserByEmailPayload&), (const override));
    MOCK_METHOD(void, updateUser, (const UpdateUserPayload&), (const override));
    MOCK_METHOD(void, deleteUser, (const DeleteUserPayload&), (const override));
};
}
