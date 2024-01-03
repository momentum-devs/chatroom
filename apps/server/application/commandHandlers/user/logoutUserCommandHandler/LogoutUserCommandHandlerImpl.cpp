#include "LogoutUserCommandHandlerImpl.h"

#include <fmt/format.h>
#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
LogoutUserCommandHandlerImpl::LogoutUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : userRepository{std::move(userRepositoryInit)}
{
}

void LogoutUserCommandHandlerImpl::execute(const LogoutUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Logging user with id \"{}\" out...", payload.id);

    const auto existingUser = userRepository->findUserById({payload.id});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id \"{}\" not found.", payload.id)};
    }

    existingUser->get()->setActive(false);

    userRepository->updateUser({**existingUser});

    LOG_S(INFO) << fmt::format("User with id \"{}\" logged out.", payload.id);
}

}
