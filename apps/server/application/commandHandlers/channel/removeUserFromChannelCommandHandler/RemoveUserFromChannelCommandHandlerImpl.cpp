#include "RemoveUserFromChannelCommandHandlerImpl.h"

#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
RemoveUserFromChannelCommandHandlerImpl::RemoveUserFromChannelCommandHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> channelRepositoryInit)
    : userChannelRepository{std::move(channelRepositoryInit)}
{
}

void RemoveUserFromChannelCommandHandlerImpl::execute(const RemoveUserFromChannelCommandHandlerPayload& payload)
{
    LOG_S(INFO) << std::format("Removing user from channel... {{userId: {}, channelId: {}}}", payload.userId,
                               payload.channelId);

    const auto existingUserChannel = userChannelRepository->findUserChannel({payload.userId, payload.channelId});

    if (!existingUserChannel)
    {
        throw errors::ResourceNotFoundError{
            std::format("UserChannel not found. {{userId: {}, channelId: {}}}", payload.userId, payload.channelId)};
    }

    userChannelRepository->deleteUserChannel({*existingUserChannel});

    LOG_S(INFO) << std::format("User removed from channel. {{userId: {}, channelId: {}}}", payload.userId,
                               payload.channelId);
}
}
