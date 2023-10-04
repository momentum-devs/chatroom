#include "FindReceivedChannelInvitationsQueryHandlerImpl.h"

namespace server::application
{
FindReceivedChannelInvitationsQueryHandlerImpl::FindReceivedChannelInvitationsQueryHandlerImpl(
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepositoryInit)
    : channelInvitationRepository{std::move(channelInvitationRepositoryInit)}
{
}

FindReceivedChannelInvitationsQueryHandlerResult FindReceivedChannelInvitationsQueryHandlerImpl::execute(
    const FindReceivedChannelInvitationsQueryHandlerPayload& payload) const
{
    const auto channelInvitations = channelInvitationRepository->findChannelInvitationsByRecipientId({payload.userId});

    return {channelInvitations};
}
}
