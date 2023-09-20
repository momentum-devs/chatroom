#pragma once

#include <string>
#include <vector>

#include "../../entities/friendship/Friendship.h"
#include "payloads/CreateFriendshipPayload.h"
#include "payloads/DeleteFriendshipPayload.h"
#include "payloads/FindFriendshipByIdPayload.h"
#include "payloads/FindFriendshipsByUserIdPayload.h"

namespace server::domain
{
class FriendshipRepository
{
public:
    virtual ~FriendshipRepository() = default;

    virtual Friendship createFriendship(const CreateFriendshipPayload&) const = 0;
    virtual std::optional<Friendship> findFriendshipById(const FindFriendshipByIdPayload&) const = 0;
    virtual std::vector<Friendship> findFriendshipsByUserId(const FindFriendshipsByUserIdPayload&) const = 0;
    virtual void deleteFriendship(const DeleteFriendshipPayload&) const = 0;
};
}
