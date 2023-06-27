#include "CreateUserChannelCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"

namespace server::application
{
CreateUserChannelCommandHandlerImpl::CreateUserChannelCommandHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit)
    : userChannelRepository{std::move(userChannelRepositoryInit)}
{
}

CreateUserChannelCommandHandlerResult
CreateUserChannelCommandHandlerImpl::execute(const CreateUserChannelCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating user channel... {{userId: {}, channelId: {}}}", payload.userId,
                               payload.channelId);

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto userChannelId = uuid.str();

    const auto userChannel =
        userChannelRepository->createUserChannel({userChannelId, payload.userId, payload.channelId});

    LOG_S(INFO) << std::format("User channel created. {{userId: {}, channelId: {}}}", userChannel.getUserId(),
                               userChannel.getChannelId());

    return {userChannel};
}
}
