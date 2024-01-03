#include "DeleteFriendshipCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteFriendshipCommandHandlerImpl::DeleteFriendshipCommandHandlerImpl(
    std::shared_ptr<domain::FriendshipRepository> friendshipRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : friendshipRepository{std::move(friendshipRepositoryInit)}, userRepository{std::move(userRepositoryInit)}
{
}

void DeleteFriendshipCommandHandlerImpl::execute(const DeleteFriendshipCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Deleting friendship... {{userId: {}, userFriendId: {}}}", payload.userId,
                               payload.userFriendId);

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.userId)};
    }

    const auto userFriend = userRepository->findUserById({payload.userFriendId});

    if (!userFriend)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.userFriendId)};
    }

    const auto friendship = friendshipRepository->findFriendshipByUserIds({payload.userId, payload.userFriendId});

    if (!friendship)
    {
        throw errors::OperationNotValidError{
            fmt::format("Friendship between user with id {} and user friend with id {} does not exist.", payload.userId,
                        payload.userFriendId)};
    }

    friendshipRepository->deleteFriendship({*friendship});

    LOG_S(INFO) << fmt::format("Friendship deleted. {{friendshipId: {}}}", friendship->getId());
}
}
