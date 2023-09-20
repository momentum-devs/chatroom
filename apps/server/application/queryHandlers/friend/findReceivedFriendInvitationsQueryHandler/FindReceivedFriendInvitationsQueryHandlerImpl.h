#pragma once

#include <memory>

#include "FindReceivedFriendInvitationsQueryHandler.h"
#include "server/domain/repositories/friendInvitationRepository/FriendInvitationRepository.h"

namespace server::application
{
class FindReceivedFriendInvitationsQueryHandlerImpl : public FindReceivedFriendInvitationsQueryHandler
{
public:
    explicit FindReceivedFriendInvitationsQueryHandlerImpl(std::shared_ptr<domain::FriendInvitationRepository>);

    FindReceivedFriendInvitationsQueryHandlerResult
    execute(const FindReceivedFriendInvitationsQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository;
};
}
