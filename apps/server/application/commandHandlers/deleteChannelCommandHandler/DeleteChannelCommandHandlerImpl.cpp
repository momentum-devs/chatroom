#include "DeleteChannelCommandHandlerImpl.h"

#include <format>

#include "../../errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteChannelCommandHandlerImpl::DeleteChannelCommandHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : channelRepository{std::move(channelRepositoryInit)}
{
}

void DeleteChannelCommandHandlerImpl::execute(const DeleteChannelCommandHandlerPayload& payload)
{
    const auto existingChannel = channelRepository->findChannelById({payload.channelId});

    if (!existingChannel)
    {
        throw errors::ResourceNotFoundError{std::format("Channel with id {} not found.", payload.channelId)};
    }

    channelRepository->deleteChannel({**existingChannel});
}
}
