#include "RegisterUserCommandHandlerImpl.h"

#include <format>

#include "loguru.hpp"

namespace server::application
{
RegisterUserCommandHandlerImpl::RegisterUserCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit, std::shared_ptr<HashService> hashServiceInit)
    : userRepository{std::move(userRepositoryInit)}, hashService{std::move(hashServiceInit)}
{
}

RegisterUserCommandHandlerResult
RegisterUserCommandHandlerImpl::execute(const RegisterUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Registering user with email \"{}\"...", payload.email);

    const auto hashedPassword = hashService->hash(payload.password);

    // TODO: generate uuid4
    const auto uuid = "9ba2c731-63c8-4bee-b8ae-9aeba3091e67";

    const auto user = userRepository->createUser({uuid, payload.email, hashedPassword, payload.email});

    LOG_S(INFO) << std::format("User with email \"{}\" registered.", payload.email);

    return {user};
}
}
