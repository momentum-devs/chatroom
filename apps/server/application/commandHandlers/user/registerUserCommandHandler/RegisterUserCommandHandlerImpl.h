#pragma once

#include <memory>

#include "RegisterUserCommandHandler.h"
#include "server/application/services/hashService/HashService.h"
#include "server/application/services/passwordValidationService/PasswordValidationService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class RegisterUserCommandHandlerImpl : public RegisterUserCommandHandler
{
public:
    RegisterUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>, std::shared_ptr<HashService>,
                                   std::shared_ptr<PasswordValidationService>);

    RegisterUserCommandHandlerResult execute(const RegisterUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<HashService> hashService;
    std::shared_ptr<PasswordValidationService> passwordValidationService;
};
}
