#include "AddUserToChannelCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
AddUserToChannelCommandHandlerImpl::AddUserToChannelCommandHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : userChannelRepository{std::move(userChannelRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      channelRepository{std::move(channelRepositoryInit)}
{
}

void AddUserToChannelCommandHandlerImpl::execute(const AddUserToChannelCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating user channel... {{userId: {}, channelId: {}}}", payload.userId,
                               payload.channelId);

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.userId)};
    }

    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{std::format("Channel with id {} not found.", payload.channelId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto userChannelId = uuid.str();

    const auto userChannel = userChannelRepository->createUserChannel({userChannelId, *user, *channel});

    LOG_S(INFO) << std::format("User channel created. {{userId: {}, channelId: {}}}", userChannel.getUser()->getId(),
                               userChannel.getChannel()->getId());
}
}
