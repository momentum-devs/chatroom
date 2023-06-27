#include "LoginUserCommandHandlerImpl.h"

#include <format>
#include <glog/logging.h>

#include "../../errors/ResourceNotFoundError.h"

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
    VLOG(0) << std::format("Logging user \"{}\" in...", payload.email);

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{std::format("User with email \"{}\" not found.", payload.email)};
    }

    const auto passwordIsValid = hashService->compare(payload.password, existingUser->getPassword());

    if (!passwordIsValid)
    {
        throw errors::ResourceNotFoundError{std::format("User with email \"{}\" not found.", payload.email)};
    }

    const auto token = tokenService->createToken(existingUser->getId());

    VLOG(0) << std::format("User with email \"{}\" logged in.", payload.email);

    return {token};
}

}
