#include "RejectFriendInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_io.hpp>
#include "fmt/format.h"

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
RejectFriendInvitationCommandHandlerImpl::RejectFriendInvitationCommandHandlerImpl(
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : friendInvitationRepository{std::move(friendInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)}
{
}

void RejectFriendInvitationCommandHandlerImpl::execute(const RejectFriendInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Rejecting friend invitation... {{recipientId: {}, friendInvitationId: {}}}",
                               payload.recipientId, payload.friendInvitationId);

    const auto friendInvitation = friendInvitationRepository->findFriendInvitationById({payload.friendInvitationId});

    if (!friendInvitation)
    {
        throw errors::ResourceNotFoundError{
            fmt::format("Friend invitation with id {} not found.", payload.friendInvitationId)};
    }

    if (payload.recipientId != friendInvitation->getRecipient()->getId())
    {
        throw errors::OperationNotValidError{
            fmt::format("User with id {} is not recipient of the friend invitation with id {}.", payload.recipientId,
                        payload.friendInvitationId)};
    }

    friendInvitationRepository->deleteFriendInvitation({*friendInvitation});

    LOG_S(INFO) << fmt::format("Friend invitation rejected. {{friendInvitationId: {}}}", friendInvitation->getId());
}
}
