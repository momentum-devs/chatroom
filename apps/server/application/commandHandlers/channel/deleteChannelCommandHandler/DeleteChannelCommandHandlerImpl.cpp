#include "DeleteChannelCommandHandlerImpl.h"

#include "fmt/format.h"
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
        throw errors::ResourceNotFoundError{fmt::format("Channel with id {} not found.", payload.channelId)};
    }

    const auto creatorId = existingChannel->get()->getCreator()->getId();

    if (payload.requesterUserId != creatorId)
    {
        throw errors::OperationNotValidError{fmt::format("User with id {} is not creator of the channel with id {}.",
                                                         payload.requesterUserId, payload.channelId)};
    }

    channelRepository->deleteChannel({**existingChannel});
}
}
