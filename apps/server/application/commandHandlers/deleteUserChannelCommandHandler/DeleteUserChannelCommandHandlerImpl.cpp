#include "DeleteUserChannelCommandHandlerImpl.h"

#include <format>

#include "../../errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteUserChannelCommandHandlerImpl::DeleteUserChannelCommandHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> channelRepositoryInit)
    : userChannelRepository{std::move(channelRepositoryInit)}
{
}

void DeleteUserChannelCommandHandlerImpl::execute(const DeleteUserChannelCommandHandlerPayload& payload)
{
    const auto existingUserChannel = userChannelRepository->fin({payload.channelId});

    if (!existingUserChannel)
    {
        throw errors::ResourceNotFoundError{std::format("UserChannel with id {} not found.", payload.channelId)};
    }

    channelRepository->deleteUserChannel({*existingUserChannel});
}
}
