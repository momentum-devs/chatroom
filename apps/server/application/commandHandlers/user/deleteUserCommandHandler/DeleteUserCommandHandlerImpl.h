#pragma once

#include <memory>

#include "DeleteUserCommandHandler.h"
#include "server/domain/repositories/groupRepository/GroupRepository.h"
#include "server/domain/repositories/userGroupRepository/UserGroupRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class DeleteUserCommandHandlerImpl : public DeleteUserCommandHandler
{
public:
    DeleteUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>, std::shared_ptr<domain::UserGroupRepository>,
                                 std::shared_ptr<domain::GroupRepository>);

    void execute(const DeleteUserCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::UserGroupRepository> userGroupRepository;
    std::shared_ptr<domain::GroupRepository> groupRepository;
};
}
