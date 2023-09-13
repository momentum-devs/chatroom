#include "AcceptFriendInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValid.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
AcceptFriendInvitationCommandHandlerImpl::AcceptFriendInvitationCommandHandlerImpl(
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : friendInvitationRepository{std::move(friendInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)}
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
        throw errors::OperationNotValid{
            std::format("User with id {} is not recipient of the friend invitation with id {}.", payload.recipientId,
                        payload.friendInvitationId)};
    }

    const auto recipientId = friendInvitation->getRecipient()->getId();
    const auto senderId = friendInvitation->getSender()->getId();

    // TODO: uncomment
    //    addUserToFriendCommandHandler->execute({recipientId, friendId});

    // TODO: consider adding transactions
    friendInvitationRepository->deleteFriendInvitation({*friendInvitation});

    LOG_S(INFO) << std::format("Friend invitation accepted. {{friendInvitationId: {}}}", friendInvitation->getId());
}
}
