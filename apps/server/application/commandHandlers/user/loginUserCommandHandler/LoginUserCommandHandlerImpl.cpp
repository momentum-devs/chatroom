#include "LoginUserCommandHandlerImpl.h"

#include "fmt/format.h"

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

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
    LOG_S(INFO) << fmt::format("Logging user \"{}\" in...", payload.email);

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with email \"{}\" not found.", payload.email)};
    }

    const auto passwordIsValid = hashService->compare(payload.password, existingUser->get()->getPassword());

    if (!passwordIsValid)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with email \"{}\" not found.", payload.email)};
    }

    existingUser->get()->setActive(true);

    userRepository->updateUser({**existingUser});

    const auto token = tokenService->createToken(existingUser->get()->getId());

    LOG_S(INFO) << fmt::format("User with email \"{}\" logged in.", payload.email);

    return {token};
}

}
