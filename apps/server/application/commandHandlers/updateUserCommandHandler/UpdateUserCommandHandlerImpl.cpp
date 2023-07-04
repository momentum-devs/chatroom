#include "UpdateUserCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
UpdateUserCommandHandlerImpl::UpdateUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : userRepository{std::move(userRepositoryInit)}
{
}

UpdateUserCommandHandlerResult
UpdateUserCommandHandlerImpl::execute(const UpdateUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Updating user with id \"{}\"...", payload.id);

    auto existingUser = userRepository->findUserById({payload.id});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{std::format("User with id \"{}\" not found.", payload.id)};
    }

    if (payload.nickname)
    {
        existingUser->get()->setNickname(*payload.nickname);
    }

    if (payload.password)
    {
        existingUser->get()->setPassword(*payload.password);
    }

    const auto user = userRepository->updateUser({**existingUser});

    LOG_S(INFO) << std::format("User with id \"{}\" updated.", payload.id);

    return {*user};
}
}
