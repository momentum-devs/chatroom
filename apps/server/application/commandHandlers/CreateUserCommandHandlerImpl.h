#pragma once

#include <memory>

#include "CreateUserCommandHandler.h"
#include "server/domain/repositories/UserRepository.h"

namespace server::application
{
class CreateUserCommandHandlerImpl: public CreateUserCommandHandler
{
public:
    explicit CreateUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>);

    CreateUserCommandHandlerResult execute(const CreateUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
