#include "DeleteUserCommandHandlerImpl.h"

#include <format>

#include "../../errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteUserCommandHandlerImpl::DeleteUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : userRepository{std::move(userRepositoryInit)}
{
}

void DeleteUserCommandHandlerImpl::execute(const DeleteUserCommandHandlerPayload& payload)
{
    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{std::format("User with email {} not found.", payload.email)};
    }

    userRepository->deleteUser({*existingUser});
}
}
