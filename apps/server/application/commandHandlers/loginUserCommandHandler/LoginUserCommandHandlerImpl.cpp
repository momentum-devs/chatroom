#include "LoginUserCommandHandlerImpl.h"

#include <format>

#include "../../errors/ResourceNotFoundError.h"
#include "loguru.hpp"

namespace server::application
{
LoginUserCommandHandlerImpl::LoginUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit,
                                                         std::shared_ptr<HashService> hashServiceInit,
                                                         std::shared_ptr<TokenService> tokenServiceInit)
    : userRepository{std::move(userRepositoryInit)},
      hashService{std::move(hashServiceInit)},
      tokenService{std::move(tokenServiceInit)}
{
}

LoginUserCommandHandlerResult LoginUserCommandHandlerImpl::execute(const LoginUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Logging user \"{}\" in...", payload.email);

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::UserNotFoundError{std::format("User with email \"{}\" not found.", payload.email)};
    }

    const auto passwordIsValid = hashService->compare(payload.password, existingUser->getPassword());

    if (!passwordIsValid)
    {
        throw errors::UserNotFoundError{std::format("User with email \"{}\" not found.", payload.email)};
    }

    const auto token = tokenService->createToken(existingUser->getId());

    LOG_S(INFO) << std::format("User with email \"{}\" logged in.", payload.email);

    return {token};
}

}
