#include "FindChannelByIdQueryHandlerImpl.h"

#include <format>

#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
FindChannelByIdQueryHandlerImpl::FindChannelByIdQueryHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : channelRepository{std::move(channelRepositoryInit)}
{
}

FindChannelByIdQueryHandlerResult
FindChannelByIdQueryHandlerImpl::execute(const FindChannelByIdQueryHandlerPayload& payload) const
{
    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{std::format("Channel with id \"{}\" not found.", payload.channelId)};
    }

    return {**channel};
}
}
