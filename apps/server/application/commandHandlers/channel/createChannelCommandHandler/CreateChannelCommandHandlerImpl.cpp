#include "CreateChannelCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateChannelCommandHandlerImpl::CreateChannelCommandHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit)
    : channelRepository{std::move(channelRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      userChannelRepository{std::move(userChannelRepositoryInit)}
{
}

CreateChannelCommandHandlerResult
CreateChannelCommandHandlerImpl::execute(const CreateChannelCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Creating channel with name \"{}\"...", payload.name);

    const auto creator = userRepository->findUserById({payload.creatorId});

    if (!creator)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.creatorId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto channelId = uuid.str();

    const auto channel = channelRepository->createChannel({channelId, payload.name, *creator, std::nullopt});

    LOG_S(INFO) << fmt::format("Channel with name \"{}\" created.", channel->getName());

    LOG_S(INFO) << fmt::format(R"(Adding "{}" user to "{}" channel...)", creator->get()->getEmail(), payload.name);

    addUserToChannel(creator->get()->getId(), channelId);

    LOG_S(INFO) << fmt::format(R"(User "{}" added to "{}" channel.)", creator->get()->getEmail(), payload.name);

    return {*channel};
}

void CreateChannelCommandHandlerImpl::addUserToChannel(const std::string& userId, const std::string& channelId) const
{
    LOG_S(INFO) << fmt::format("Adding user to channel... {{userId: {}, channelId: {}}}", userId, channelId);

    const auto user = userRepository->findUserById({userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", userId)};
    }

    const auto channel = channelRepository->findChannelById({channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{fmt::format("Channel with id {} not found.", channelId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto userChannelId = uuid.str();

    const auto userChannel = userChannelRepository->createUserChannel({userChannelId, *user, *channel});

    LOG_S(INFO) << fmt::format("User added to channel. {{userId: {}, channelId: {}}}", userChannel.getUser()->getId(),
                               userChannel.getChannel()->getId());
}

}
