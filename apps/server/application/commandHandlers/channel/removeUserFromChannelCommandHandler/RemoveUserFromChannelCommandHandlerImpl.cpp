#include "RemoveUserFromChannelCommandHandlerImpl.h"

#include <fmt/format.h>
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
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
    LOG_S(INFO) << fmt::format("Removing user from channel... {{userId: {}, channelId: {}, requesterUserId: {}}}",
                               payload.userId, payload.channelId, payload.requesterUserId);

    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{fmt::format("Channel not found. {{channelId: {}}}", payload.channelId)};
    }

    const auto existingUserChannel = userChannelRepository->findUserChannel({payload.userId, payload.channelId});

    if (!existingUserChannel)
    {
        throw errors::ResourceNotFoundError{
            fmt::format("UserChannel not found. {{userId: {}, channelId: {}}}", payload.userId, payload.channelId)};
    }

    if (channel->get()->getCreator()->getId() != payload.requesterUserId)
    {
        throw errors::OperationNotValidError{
            fmt::format("User with id {} cannot remove users because he is not creator of the channel with id {}.",
                        payload.requesterUserId, payload.channelId)};
    }

    userChannelRepository->deleteUserChannel({*existingUserChannel});

    LOG_S(INFO) << fmt::format("User removed from channel. {{userId: {}, channelId: {}}}", payload.userId,
                               payload.channelId);
}
}
