#include "AcceptChannelInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValid.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
AcceptChannelInvitationCommandHandlerImpl::AcceptChannelInvitationCommandHandlerImpl(
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<application::AddUserToChannelCommandHandler> addUserToChannelCommandHandlerInit)
    : channelInvitationRepository{std::move(channelInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      addUserToChannelCommandHandler{std::move(addUserToChannelCommandHandlerInit)}
{
}

void AcceptChannelInvitationCommandHandlerImpl::execute(
    const AcceptChannelInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Accepting channel invitation... {{recipientId: {}, channelInvitationId: {}}}",
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
        throw errors::OperationNotValid{
            std::format("User with id {} is not recipient of the channel invitation with id {}.", payload.recipientId,
                        payload.channelInvitationId)};
    }

    const auto recipientId = channelInvitation->getRecipient()->getId();
    const auto channelId = channelInvitation->getChannel()->getId();

    addUserToChannelCommandHandler->execute({recipientId, channelId});

    // TODO: consider adding transactions
    channelInvitationRepository->deleteChannelInvitation({*channelInvitation});

    LOG_S(INFO) << std::format("Channel invitation accepted. {{channelInvitationId: {}}}", channelInvitation->getId());
}
}
