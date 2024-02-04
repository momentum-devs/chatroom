#pragma once

#include <memory>

#include "ChangeUserPasswordCommandHandler.h"
#include "server/application/services/hashService/HashService.h"
#include "server/application/services/passwordValidationService/PasswordValidationService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class ChangeUserPasswordCommandHandlerImpl : public ChangeUserPasswordCommandHandler
{
public:
    ChangeUserPasswordCommandHandlerImpl(std::shared_ptr<domain::UserRepository>, std::shared_ptr<HashService>,
                                         std::shared_ptr<PasswordValidationService>);

    void execute(const ChangeUserPasswordCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<HashService> hashService;
    std::shared_ptr<PasswordValidationService> passwordValidationService;
};
}
