#include "AcceptFriendInvitationCommandHandlerImpl.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
AcceptFriendInvitationCommandHandlerImpl::AcceptFriendInvitationCommandHandlerImpl(
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::FriendshipRepository> friendshipRepositoryInit)
    : friendInvitationRepository{std::move(friendInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      friendshipRepository{std::move(friendshipRepositoryInit)}
{
}

void AcceptFriendInvitationCommandHandlerImpl::execute(const AcceptFriendInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Accepting friend invitation... {{recipientId: {}, friendInvitationId: {}}}",
                               payload.recipientId, payload.friendInvitationId);

    const auto friendInvitation = friendInvitationRepository->findFriendInvitationById({payload.friendInvitationId});

    if (!friendInvitation)
    {
        throw errors::ResourceNotFoundError{
            std::format("Friend invitation with id {} not found.", payload.friendInvitationId)};
    }

    if (payload.recipientId != friendInvitation->getRecipient()->getId())
    {
        throw errors::OperationNotValidError{
            std::format("User with id {} is not recipient of the friend invitation with id {}.", payload.recipientId,
                        payload.friendInvitationId)};
    }

    const auto recipientId = friendInvitation->getRecipient()->getId();
    const auto senderId = friendInvitation->getSender()->getId();

    // TODO: consider adding transactions

    createFriendship(senderId, recipientId);

    friendInvitationRepository->deleteFriendInvitation({*friendInvitation});

    LOG_S(INFO) << std::format("Friend invitation accepted. {{friendInvitationId: {}}}", friendInvitation->getId());
}

void AcceptFriendInvitationCommandHandlerImpl::createFriendship(const std::string& userId,
                                                                const std::string& userFriendId) const
{
    LOG_S(INFO) << std::format("Creating friendship... {{userId: {}, userFriendId: {}}}", userId, userFriendId);

    const auto user = userRepository->findUserById({userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", userId)};
    }

    const auto userFriend = userRepository->findUserById({userFriendId});

    if (!userFriend)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", userFriendId)};
    }

    const auto existingFriendship = friendshipRepository->findFriendshipByUserIds({userId, userFriendId});

    if (existingFriendship)
    {
        throw errors::OperationNotValidError{std::format(
            "Friendship between user with id {} and user friend with id {} already exists.", userId, userFriendId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto friendshipId = uuid.str();

    const auto friendship = friendshipRepository->createFriendship({friendshipId, *user, *userFriend});

    LOG_S(INFO) << std::format("Friendship created. {{friendshipId: {}}}", friendship.getId());
}
}
