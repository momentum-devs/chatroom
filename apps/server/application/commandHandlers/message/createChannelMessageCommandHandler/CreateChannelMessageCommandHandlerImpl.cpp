#include "CreateChannelMessageCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fmt/format.h>
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateChannelMessageCommandHandlerImpl::CreateChannelMessageCommandHandlerImpl(
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

CreateChannelMessageCommandHandlerResult
CreateChannelMessageCommandHandlerImpl::execute(const CreateChannelMessageCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format(R"(Creating channel message "{}" by sender with id "{}" to channel with id "{}"...)",
                               payload.content, payload.senderId, payload.channelId);

    const auto sender = userRepository->findUserById({payload.senderId});

    if (!sender)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.senderId)};
    }

    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{fmt::format("Channel with id {} not found.", payload.channelId)};
    }

    const auto userChannels = userChannelRepository->findUsersChannelsByUserId({payload.senderId});

    const auto userIsMemberOfChannel = std::any_of(userChannels.begin(), userChannels.end(),
                                                   [&payload](const domain::UserChannel& userChannel)
                                                   {
                                                       return userChannel.getUser()->getId() == payload.senderId &&
                                                              userChannel.getChannel()->getId() == payload.channelId;
                                                   });

    if (!userIsMemberOfChannel)
    {
        throw errors::OperationNotValidError{fmt::format("Sender {} is not a member of channel {}.",
                                                         sender->get()->getEmail(), channel->get()->getName())};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto messageId = uuid.str();

    const auto message = messageRepository->createMessage({messageId, payload.content, *sender, channel, std::nullopt});

    LOG_S(INFO) << fmt::format(R"(Message from sender with id "{}" with content "{}" created.)", payload.senderId,
                               payload.content);

    return {*message};
}

}
