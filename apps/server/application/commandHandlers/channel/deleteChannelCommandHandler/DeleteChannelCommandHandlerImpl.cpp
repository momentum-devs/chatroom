#include "DeleteChannelCommandHandlerImpl.h"

#include <format>

#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

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

    const auto creatorId = existingChannel->get()->getCreatorId();

    if (payload.requesterUserId != creatorId)
    {
        throw errors::OperationNotValidError{std::format("User with id {} is not creator of the channel with id {}.",
                                                         payload.requesterUserId, payload.channelId)};
    }

    channelRepository->deleteChannel({**existingChannel});
}
}