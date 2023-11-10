#pragma once

#include <memory>

#include "CreateGroupCommandHandler.h"
#include "server/domain/repositories/groupRepository/GroupRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateGroupCommandHandlerImpl : public CreateGroupCommandHandler
{
public:
    explicit CreateGroupCommandHandlerImpl(std::shared_ptr<domain::GroupRepository>);

    CreateGroupCommandHandlerResult execute() const override;

private:
    std::shared_ptr<domain::GroupRepository> groupRepository;
};
}
