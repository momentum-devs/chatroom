#pragma once

#include <memory>

#include "AcceptFriendInvitationCommandHandler.h"
#include "server/domain/repositories/friendInvitationRepository/FriendInvitationRepository.h"
#include "server/domain/repositories/friendshipRepository/FriendshipRepository.h"
#include "server/domain/repositories/groupRepository/GroupRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class AcceptFriendInvitationCommandHandlerImpl : public AcceptFriendInvitationCommandHandler
{
public:
    AcceptFriendInvitationCommandHandlerImpl(std::shared_ptr<domain::FriendInvitationRepository>,
                                             std::shared_ptr<domain::UserRepository>,
                                             std::shared_ptr<domain::FriendshipRepository>,
                                             std::shared_ptr<domain::GroupRepository>);

    void execute(const AcceptFriendInvitationCommandHandlerPayload&) const override;

private:
    void createFriendship(const std::string& userId, const std::string& userFriendId) const;

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::FriendshipRepository> friendshipRepository;
    std::shared_ptr<domain::GroupRepository> groupRepository;
};
}
