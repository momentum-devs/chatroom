#include "DeleteUserCommandHandlerImpl.h"

#include <format>

#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteUserCommandHandlerImpl::DeleteUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : userRepository{std::move(userRepositoryInit)}
{
}

void DeleteUserCommandHandlerImpl::execute(const DeleteUserCommandHandlerPayload& payload)
{
    const auto existingUser = userRepository->findUserById({payload.id});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.id)};
    }

    userRepository->deleteUser({**existingUser});
}
}
