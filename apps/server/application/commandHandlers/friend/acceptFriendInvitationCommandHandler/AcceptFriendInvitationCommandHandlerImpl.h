#pragma once

#include <memory>

#include "AcceptFriendInvitationCommandHandler.h"
#include "server/application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandler.h"
#include "server/domain/repositories/friendInvitationRepository/FriendInvitationRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class AcceptFriendInvitationCommandHandlerImpl : public AcceptFriendInvitationCommandHandler
{
public:
    AcceptFriendInvitationCommandHandlerImpl(std::shared_ptr<domain::FriendInvitationRepository>,
                                             std::shared_ptr<domain::UserRepository>,
                                             std::shared_ptr<application::CreateFriendshipCommandHandler>);

    void execute(const AcceptFriendInvitationCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<application::CreateFriendshipCommandHandler> createFriendshipCommandHandler;
};
}
