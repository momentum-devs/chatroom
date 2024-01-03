#include "UpdateUserCommandHandlerImpl.h"

#include <boost/uuid/uuid_io.hpp>

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
UpdateUserCommandHandlerImpl::UpdateUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository> userRepositoryInit,
                                                           std::shared_ptr<HashService> hashServiceInit)
    : userRepository{std::move(userRepositoryInit)}, hashService{std::move(hashServiceInit)}
{
}

UpdateUserCommandHandlerResult
UpdateUserCommandHandlerImpl::execute(const UpdateUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Updating user with id \"{}\"...", payload.id);

    auto existingUser = userRepository->findUserById({payload.id});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id \"{}\" not found.", payload.id)};
    }

    if (payload.nickname)
    {
        existingUser->get()->setNickname(*payload.nickname);
    }

    if (payload.password)
    {
        const auto hashedPassword = hashService->hash(*payload.password);

        existingUser->get()->setPassword(hashedPassword);
    }

    const auto user = userRepository->updateUser({**existingUser});

    LOG_S(INFO) << fmt::format("User with id \"{}\" updated.", payload.id);

    return {*user};
}
}
