#pragma once

#include <memory>

#include "AcceptFriendInvitationCommandHandler.h"
#include "server/domain/repositories/friendInvitationRepository/FriendInvitationRepository.h"
#include "server/domain/repositories/friendshipRepository/FriendshipRepository.h"
#include "server/domain/repositories/groupRepository/GroupRepository.h"
#include "server/domain/repositories/userGroupRepository/UserGroupRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class AcceptFriendInvitationCommandHandlerImpl : public AcceptFriendInvitationCommandHandler
{
public:
    AcceptFriendInvitationCommandHandlerImpl(std::shared_ptr<domain::FriendInvitationRepository>,
                                             std::shared_ptr<domain::UserRepository>,
                                             std::shared_ptr<domain::FriendshipRepository>,
                                             std::shared_ptr<domain::GroupRepository>,
                                             std::shared_ptr<domain::UserGroupRepository>);

    void execute(const AcceptFriendInvitationCommandHandlerPayload&) const override;

private:
    void createFriendship(const std::string& userId, const std::string& userFriendId) const;
    void createUserGroup(const std::shared_ptr<domain::User>& user, const std::shared_ptr<domain::Group>& group) const;

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::FriendshipRepository> friendshipRepository;
    std::shared_ptr<domain::GroupRepository> groupRepository;
    std::shared_ptr<domain::UserGroupRepository> userGroupRepository;
};
}
