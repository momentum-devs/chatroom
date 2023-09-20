#pragma once

#include <memory>

#include "RegisterUserCommandHandler.h"
#include "server/application/services/hashService/HashService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class RegisterUserCommandHandlerImpl : public RegisterUserCommandHandler
{
public:
    RegisterUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>, std::shared_ptr<HashService>);

    RegisterUserCommandHandlerResult execute(const RegisterUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<HashService> hashService;
};
}
