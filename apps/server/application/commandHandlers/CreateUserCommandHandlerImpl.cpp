#include "CreateUserCommandHandlerImpl.h"

#include <format>

#include "loguru.hpp"

namespace server::application
{
CreateUserCommandHandlerImpl::CreateUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit) : userRepository{std::move(userRepositoryInit)}{}

CreateUserCommandHandlerResult CreateUserCommandHandlerImpl::execute(const CreateUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating user with email \"{}\"...", payload.email);

    const auto user = userRepository->createUser({payload.email, payload.password, payload.email});

    LOG_S(INFO) << std::format("User with email \"{}\" created.", payload.email);

    return {user};
}
}
