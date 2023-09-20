#pragma once

#include <gmock/gmock.h>

#include "FriendshipRepository.h"

namespace server::domain
{
class FriendshipRepositoryMock : public FriendshipRepository
{
public:
    MOCK_METHOD(Friendship, createFriendship, (const CreateFriendshipPayload&), (const override));
    MOCK_METHOD(std::optional<Friendship>, findFriendshipById, (const FindFriendshipByIdPayload&), (const override));
    MOCK_METHOD(std::vector<Friendship>, findFriendshipsByUserId, (const FindFriendshipsByUserIdPayload&),
                (const override));
    MOCK_METHOD(void, deleteFriendship, (const DeleteFriendshipPayload&), (const override));
};
}
