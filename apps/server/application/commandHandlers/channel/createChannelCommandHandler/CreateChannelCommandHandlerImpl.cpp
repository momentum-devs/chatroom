#include "CreateChannelCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateChannelCommandHandlerImpl::CreateChannelCommandHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit,
    std::shared_ptr<AddUserToChannelCommandHandler> addUserToChannelCommandHandlerInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : channelRepository{std::move(channelRepositoryInit)},
      addUserToChannelCommandHandler{std::move(addUserToChannelCommandHandlerInit)},
      userRepository{std::move(userRepositoryInit)}
{
}

CreateChannelCommandHandlerResult
CreateChannelCommandHandlerImpl::execute(const CreateChannelCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating channel with name \"{}\"...", payload.name);

    const auto creator = userRepository->findUserById({payload.creatorId});

    if (!creator)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.creatorId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto channelId = uuid.str();

    const auto channel = channelRepository->createChannel({channelId, payload.name, *creator});

    LOG_S(INFO) << std::format("Channel with name \"{}\" created.", channel->getName());

    LOG_S(INFO) << std::format(R"(Adding "{}" user to "{}" channel...)", creator->get()->getEmail(), payload.name);

    addUserToChannelCommandHandler->execute({payload.creatorId, channelId});

    LOG_S(INFO) << std::format(R"(User "{}" added to "{}" channel.)", creator->get()->getEmail(), payload.name);

    return {*channel};
}
}
