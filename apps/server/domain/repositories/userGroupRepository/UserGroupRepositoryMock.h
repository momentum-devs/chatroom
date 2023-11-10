#pragma once

#include <gmock/gmock.h>

#include "UserGroupRepository.h"

namespace server::domain
{
class UserGroupRepositoryMock : public UserGroupRepository
{
public:
    MOCK_METHOD(UserGroup, createUserGroup, (const CreateUserGroupPayload&), (const override));
    MOCK_METHOD(std::optional<UserGroup>, findUserGroupById, (const FindUserGroupByIdPayload&), (const override));
    MOCK_METHOD(std::vector<UserGroup>, findUsersGroupsByUserId, (const FindUsersGroupsByUserIdPayload&),
                (const override));
    MOCK_METHOD(std::vector<UserGroup>, findUsersGroupsByGroupId, (const FindUsersGroupsByGroupIdPayload&),
                (const override));
    MOCK_METHOD(void, deleteUserGroup, (const DeleteUserGroupPayload&), (const override));
};
}
