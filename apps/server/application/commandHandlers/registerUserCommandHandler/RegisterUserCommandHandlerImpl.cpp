#include "RegisterUserCommandHandlerImpl.h"

#include <format>

#include "loguru.hpp"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto userId = uuid.str();

    const auto user = userRepository->createUser({userId, payload.email, hashedPassword, payload.email});

    LOG_S(INFO) << std::format("User with email \"{}\" registered.", payload.email);

    return {user};
}
}
