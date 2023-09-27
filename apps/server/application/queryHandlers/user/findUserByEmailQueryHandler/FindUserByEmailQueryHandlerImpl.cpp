#include "FindUserByEmailQueryHandlerImpl.h"

#include <format>

#include "server/application/errors/ResourceNotFoundError.h"
#include "User.h"

namespace server::application
{
FindUserByEmailQueryHandlerImpl::FindUserByEmailQueryHandlerImpl(
    std::shared_ptr<domain::UserRepository> userChannelRepositoryInit)
    : userRepository{std::move(userChannelRepositoryInit)}
{
}

FindUserByEmailQueryHandlerResult
FindUserByEmailQueryHandlerImpl::execute(const FindUserByEmailQueryHandlerPayload& payload) const
{
    const auto user = userRepository->findUserByEmail({payload.email});

    if (!user)
    {
        throw errors::ResourceNotFoundError{std::format("User with email \"{}\" not found.", payload.email)};
    }

    return {**user};
}
}
