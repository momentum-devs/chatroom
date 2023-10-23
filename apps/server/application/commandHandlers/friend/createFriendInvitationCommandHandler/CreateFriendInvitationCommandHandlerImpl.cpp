#include "CreateFriendInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateFriendInvitationCommandHandlerImpl::CreateFriendInvitationCommandHandlerImpl(
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::FriendshipRepository> friendshipRepositoryInit)
    : friendInvitationRepository{std::move(friendInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      friendshipRepository{std::move(friendshipRepositoryInit)}
{
}

void CreateFriendInvitationCommandHandlerImpl::execute(const CreateFriendInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating friend invitation... {{senderId: {}, recipientId: {}}}", payload.senderId,
                               payload.recipientId);

    if (payload.senderId == payload.recipientId)
    {
        throw errors::OperationNotValidError{std::format(
            "Friend invitation's sender and recipient cannot be the same user. {{senderId: {}, recipientId: {}}}.",
            payload.senderId, payload.recipientId)};
    }

    const auto sender = userRepository->findUserById({payload.senderId});

    if (!sender)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.senderId)};
    }

    const auto recipient = userRepository->findUserById({payload.recipientId});

    if (!recipient)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.recipientId)};
    }

    const auto existingFriendInvitation =
        friendInvitationRepository->findFriendInvitation({payload.senderId, payload.recipientId});

    if (existingFriendInvitation)
    {
        throw errors::OperationNotValidError{
            std::format("Friend invitation sent by user with id {} to user with id {} already exists.",
                        payload.senderId, payload.recipientId)};
    }

    const auto existingFriendship =
        friendshipRepository->findFriendshipByUserIds({payload.senderId, payload.recipientId});

    if (existingFriendship)
    {
        throw errors::OperationNotValidError{
            std::format("Friendship between user with id {} and user friend with id {} already exists.",
                        payload.senderId, payload.recipientId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto friendInvitationId = uuid.str();

    const auto friendInvitation =
        friendInvitationRepository->createFriendInvitation({friendInvitationId, *sender, *recipient});

    LOG_S(INFO) << std::format("Friend invitation created. {{friendInvitationId: {}}}", friendInvitation.getId());
}
}
