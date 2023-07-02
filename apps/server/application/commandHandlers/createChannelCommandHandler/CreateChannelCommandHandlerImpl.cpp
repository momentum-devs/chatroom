#include "CreateChannelCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"

namespace server::application
{
CreateChannelCommandHandlerImpl::CreateChannelCommandHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : channelRepository{std::move(channelRepositoryInit)}
{
}

CreateChannelCommandHandlerResult
CreateChannelCommandHandlerImpl::execute(const CreateChannelCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating channel with name \"{}\"...", payload.name);

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto channelId = uuid.str();

    const auto channel = channelRepository->createChannel({channelId, payload.name, payload.creatorId});

    LOG_S(INFO) << std::format("Channel with name \"{}\" created.", channel->getName());

    return {*channel};
}
}
