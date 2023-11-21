#pragma once

#include <gmock/gmock.h>

#include "UserChannelRepository.h"

namespace server::domain
{
class UserChannelRepositoryMock : public UserChannelRepository
{
public:
    MOCK_METHOD(UserChannel, createUserChannel, (const CreateUserChannelPayload&), (const override));
    MOCK_METHOD(std::optional<UserChannel>, findUserChannelById, (const FindUserChannelByIdPayload&), (const override));
    MOCK_METHOD(std::vector<UserChannel>, findUsersChannelsByUserId, (const FindUsersChannelsByUserIdPayload&),
                (const override));
    MOCK_METHOD(std::vector<UserChannel>, findUsersChannelsByChannelId, (const FindUsersChannelsByChannelIdPayload&),
                (const override));
    MOCK_METHOD(UserChannel, updateUserChannel, (const UpdateUserChannelPayload&), (const override));
    MOCK_METHOD(void, deleteUserChannel, (const DeleteUserChannelPayload&), (const override));
};
}
