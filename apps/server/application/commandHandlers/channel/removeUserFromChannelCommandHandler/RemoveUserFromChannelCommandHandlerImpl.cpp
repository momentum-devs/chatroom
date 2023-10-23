#include "RemoveUserFromChannelCommandHandlerImpl.h"

#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
RemoveUserFromChannelCommandHandlerImpl::RemoveUserFromChannelCommandHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit,
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : userChannelRepository{std::move(userChannelRepositoryInit)}, channelRepository{std::move(channelRepositoryInit)}
{
}

void RemoveUserFromChannelCommandHandlerImpl::execute(const RemoveUserFromChannelCommandHandlerPayload& payload)
{
    LOG_S(INFO) << std::format("Removing user from channel... {{userId: {}, channelId: {}}}", payload.userId,
                               payload.channelId);

    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{
            std::format("Channel not found. {{channelId: {}}}", payload.channelId)};
    }

    const auto existingUserChannel = userChannelRepository->findUserChannel({payload.userId, payload.channelId});

    if (!existingUserChannel)
    {
        throw errors::ResourceNotFoundError{
            std::format("UserChannel not found. {{userId: {}, channelId: {}}}", payload.userId, payload.channelId)};
    }

    userChannelRepository->deleteUserChannel({*existingUserChannel});

    const auto channelCreatorId = channel->get()->getCreator()->getId();

    if (channelCreatorId == payload.userId)
    {
        channelRepository->deleteChannel({**channel});

        LOG_S(INFO) << std::format("Channel deleted. {{channelId: {}}}", payload.channelId);
    }

    LOG_S(INFO) << std::format("User removed from channel. {{userId: {}, channelId: {}}}", payload.userId,
                               payload.channelId);
}
}
