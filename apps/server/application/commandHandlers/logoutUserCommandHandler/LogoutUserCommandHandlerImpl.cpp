#include "LogoutUserCommandHandlerImpl.h"

#include <format>

#include "../../errors/ResourceNotFoundError.h"
#include "loguru.hpp"

namespace server::application
{
LogoutUserCommandHandlerImpl::LogoutUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : userRepository{std::move(userRepositoryInit)}
{
}

void LogoutUserCommandHandlerImpl::execute(const LogoutUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Logging user with id \"{}\" out...", payload.id);

    const auto existingUser = userRepository->findUserById({payload.id});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{std::format("User with id \"{}\" not found.", payload.id)};
    }

    existingUser->get()->setActive(false);

    userRepository->updateUser({**existingUser});

    LOG_S(INFO) << std::format("User with id \"{}\" logged out.", payload.id);
}

}
