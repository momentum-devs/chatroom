#pragma once

#include <memory>

#include "LoginUserCommandHandler.h"
#include "server/application/services/hashService/HashService.h"
#include "server/application/services/tokenService/TokenService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class LoginUserCommandHandlerImpl : public LoginUserCommandHandler
{
public:
    LoginUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>, std::shared_ptr<HashService>,
                                std::shared_ptr<TokenService>);

    LoginUserCommandHandlerResult execute(const LoginUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<HashService> hashService;
    std::shared_ptr<TokenService> tokenService;
};
}
