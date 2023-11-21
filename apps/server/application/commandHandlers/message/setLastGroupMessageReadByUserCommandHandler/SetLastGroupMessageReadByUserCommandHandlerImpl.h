#pragma once

#include <memory>

#include "server/domain/repositories/groupRepository/GroupRepository.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"
#include "server/domain/repositories/userGroupRepository/UserGroupRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"
#include "SetLastGroupMessageReadByUserCommandHandler.h"

namespace server::application
{
class SetLastGroupMessageReadByUserCommandHandlerImpl : public SetLastGroupMessageReadByUserCommandHandler
{
public:
    SetLastGroupMessageReadByUserCommandHandlerImpl(std::shared_ptr<domain::GroupRepository>,
                                                      std::shared_ptr<domain::UserRepository>,
                                                      std::shared_ptr<domain::UserGroupRepository>,
                                                      std::shared_ptr<domain::MessageRepository>);

    void execute(const SetLastGroupMessageReadByUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::GroupRepository> groupRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::UserGroupRepository> userGroupRepository;
    std::shared_ptr<domain::MessageRepository> messageRepository;
};
}
