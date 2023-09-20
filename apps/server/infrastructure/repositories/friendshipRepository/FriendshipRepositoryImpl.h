#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "server/domain/repositories/friendshipRepository/FriendshipRepository.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class FriendshipRepositoryImpl : public domain::FriendshipRepository
{
public:
    FriendshipRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::shared_ptr<FriendshipMapper>,
                             std::shared_ptr<UserMapper>);

    domain::Friendship createFriendship(const domain::CreateFriendshipPayload&) const;
    std::optional<domain::Friendship> findFriendshipById(const domain::FindFriendshipByIdPayload&) const;
    std::vector<domain::Friendship> findFriendshipsByUserId(const domain::FindFriendshipsByUserIdPayload&) const;
    void deleteFriendship(const domain::DeleteFriendshipPayload&) const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<FriendshipMapper> friendshipMapper;
    std::shared_ptr<UserMapper> userMapper;
};
}
