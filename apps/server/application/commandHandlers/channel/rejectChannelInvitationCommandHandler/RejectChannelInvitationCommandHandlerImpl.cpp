#include "RejectChannelInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
RejectChannelInvitationCommandHandlerImpl::RejectChannelInvitationCommandHandlerImpl(
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : channelInvitationRepository{std::move(channelInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)}
{
}

void RejectChannelInvitationCommandHandlerImpl::execute(
    const RejectChannelInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Rejecting channel invitation... {{recipientId: {}, channelInvitationId: {}}}",
                               payload.recipientId, payload.channelInvitationId);

    const auto channelInvitation =
        channelInvitationRepository->findChannelInvitationById({payload.channelInvitationId});

    if (!channelInvitation)
    {
        throw errors::ResourceNotFoundError{
            std::format("Channel invitation with id {} not found.", payload.channelInvitationId)};
    }

    if (payload.recipientId != channelInvitation->getRecipient()->getId())
    {
        throw errors::OperationNotValidError{
            std::format("User with id {} is not recipient of the channel invitation with id {}.", payload.recipientId,
                        payload.channelInvitationId)};
    }

    channelInvitationRepository->deleteChannelInvitation({*channelInvitation});

    LOG_S(INFO) << std::format("Channel invitation rejected. {{channelInvitationId: {}}}", channelInvitation->getId());
}
}
