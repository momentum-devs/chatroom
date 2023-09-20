#include "CreateFriendshipCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateFriendshipCommandHandlerImpl::CreateFriendshipCommandHandlerImpl(
    std::shared_ptr<domain::FriendshipRepository> friendshipRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : friendshipRepository{std::move(friendshipRepositoryInit)}, userRepository{std::move(userRepositoryInit)}
{
}

void CreateFriendshipCommandHandlerImpl::execute(const CreateFriendshipCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating friendship... {{userId: {}, userFriendId: {}}}", payload.userId,
                               payload.userFriendId);

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.userId)};
    }

    const auto userFriend = userRepository->findUserById({payload.userFriendId});

    if (!userFriend)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.userFriendId)};
    }

    const auto existingFriendship =
        friendshipRepository->findFriendshipByUserIds({payload.userId, payload.userFriendId});

    if (existingFriendship)
    {
        throw errors::OperationNotValidError{
            std::format("Friendship between user with id {} and user friend with id {} already exists.", payload.userId,
                        payload.userFriendId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto friendshipId = uuid.str();

    const auto friendship = friendshipRepository->createFriendship({friendshipId, *user, *userFriend});

    LOG_S(INFO) << std::format("Friendship created. {{friendshipId: {}}}", friendship.getId());
}
}
