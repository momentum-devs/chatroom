#include "RegisterUserCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceAlreadyExistsError.h"

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

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (existingUser)
    {
        throw errors::ResourceAlreadyExistsError{std::format("User with email \"{}\" already exists.", payload.email)};
    }

    const auto hashedPassword = hashService->hash(payload.password);

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto userId = uuid.str();

    const auto user = userRepository->createUser({userId, payload.email, hashedPassword, payload.email, false, false});

    LOG_S(INFO) << std::format("User with email \"{}\" registered.", payload.email);

    return {*user};
}
}
