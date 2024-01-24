#pragma once

#include <memory>

#include "server/application/services/hashService/HashService.h"
#include "server/application/services/passwordValidationService/PasswordValidationService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"
#include "UpdateUserCommandHandler.h"

namespace server::application
{
class UpdateUserCommandHandlerImpl : public UpdateUserCommandHandler
{
public:
    UpdateUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>, std::shared_ptr<HashService>,
                                 std::shared_ptr<PasswordValidationService>);

    UpdateUserCommandHandlerResult execute(const UpdateUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<HashService> hashService;
    std::shared_ptr<PasswordValidationService> passwordValidationService;
};
}
