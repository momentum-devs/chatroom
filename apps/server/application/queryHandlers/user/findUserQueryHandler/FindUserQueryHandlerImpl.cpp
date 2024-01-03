#include "FindUserQueryHandlerImpl.h"

#include "fmt/format.h"

#include "server/application/errors/ResourceNotFoundError.h"
#include "User.h"

namespace server::application
{
FindUserQueryHandlerImpl::FindUserQueryHandlerImpl(std::shared_ptr<domain::UserRepository> userChannelRepositoryInit)
    : userRepository{std::move(userChannelRepositoryInit)}
{
}

FindUserQueryHandlerResult FindUserQueryHandlerImpl::execute(const FindUserQueryHandlerPayload& payload) const
{
    const auto user = userRepository->findUserById({payload.id});

    if (!user)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id \"{}\" not found.", payload.id)};
    }

    return {**user};
}
}
