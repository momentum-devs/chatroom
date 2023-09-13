#pragma once

#include <memory>

#include "RejectFriendInvitationCommandHandler.h"
#include "server/domain/repositories/friendInvitationRepository/FriendInvitationRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class RejectFriendInvitationCommandHandlerImpl : public RejectFriendInvitationCommandHandler
{
public:
    RejectFriendInvitationCommandHandlerImpl(std::shared_ptr<domain::FriendInvitationRepository>,
                                             std::shared_ptr<domain::UserRepository>);

    void execute(const RejectFriendInvitationCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
