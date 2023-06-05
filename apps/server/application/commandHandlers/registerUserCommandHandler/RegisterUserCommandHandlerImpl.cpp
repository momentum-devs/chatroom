#include <format>

#include "loguru.hpp"
#include "RegisterUserCommandHandlerImpl.h"

namespace server::application
{
RegisterUserCommandHandlerImpl::RegisterUserCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : userRepository{std::move(userRepositoryInit)}
{
}

RegisterUserCommandHandlerResult
RegisterUserCommandHandlerImpl::execute(const RegisterUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Registering user with email \"{}\"...", payload.email);

    const auto user = userRepository->createUser({payload.email, payload.password, payload.email});

    LOG_S(INFO) << std::format("User with email \"{}\" registered.", payload.email);

    return {user};
}
}
