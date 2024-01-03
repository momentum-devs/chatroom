#include "SetLastChannelMessageReadByUserCommandHandlerImpl.h"

#include <fmt/format.h>
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
SetLastChannelMessageReadByUserCommandHandlerImpl::SetLastChannelMessageReadByUserCommandHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit,
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit)
    : channelRepository{std::move(channelRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      userChannelRepository{std::move(userChannelRepositoryInit)},
      messageRepository{std::move(messageRepositoryInit)}
{
}

void SetLastChannelMessageReadByUserCommandHandlerImpl::execute(
    const SetLastChannelMessageReadByUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format(
        "Setting last channel message read by user... {{userId: {}, channelId: {}, messageId: {}}}", payload.userId,
        payload.channelId, payload.messageId);

    const auto message = messageRepository->findMessageById({payload.messageId});

    if (!message)
    {
        throw errors::ResourceNotFoundError{fmt::format("Message with id {} not found.", payload.messageId)};
    }

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.userId)};
    }

    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{fmt::format("Channel with id {} not found.", payload.channelId)};
    }

    auto userChannel = userChannelRepository->findUserChannel({payload.userId, payload.channelId});

    if (!userChannel)
    {
        throw errors::ResourceNotFoundError{
            fmt::format("UserChannel not found. {{userId: {}, channelId: {}}}", payload.userId, payload.channelId)};
    }

    userChannel->setLastReadMessageId(payload.messageId);

    userChannelRepository->updateUserChannel({*userChannel});

    LOG_S(INFO) << fmt::format("Last read channel message set. {{userId: {}, channelId: {}, messageId: {}}}",
                               payload.userId, payload.channelId, payload.messageId);
}

}
