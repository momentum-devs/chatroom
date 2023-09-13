#pragma once

#include <memory>

#include "CreateFriendInvitationCommandHandler.h"
#include "server/domain/repositories/friendInvitationRepository/FriendInvitationRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateFriendInvitationCommandHandlerImpl : public CreateFriendInvitationCommandHandler
{
public:
    CreateFriendInvitationCommandHandlerImpl(std::shared_ptr<domain::FriendInvitationRepository>,
                                             std::shared_ptr<domain::UserRepository>);

    void execute(const CreateFriendInvitationCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
