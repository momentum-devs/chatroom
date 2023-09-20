#include "FindReceivedFriendInvitationsQueryHandlerImpl.h"

namespace server::application
{
FindReceivedFriendInvitationsQueryHandlerImpl::FindReceivedFriendInvitationsQueryHandlerImpl(
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepositoryInit)
    : friendInvitationRepository{std::move(friendInvitationRepositoryInit)}
{
}

FindReceivedFriendInvitationsQueryHandlerResult FindReceivedFriendInvitationsQueryHandlerImpl::execute(
    const FindReceivedFriendInvitationsQueryHandlerPayload& payload) const
{
    const auto friendInvitations = friendInvitationRepository->findFriendInvitationsByRecipientId({payload.userId});

    return {friendInvitations};
}
}
