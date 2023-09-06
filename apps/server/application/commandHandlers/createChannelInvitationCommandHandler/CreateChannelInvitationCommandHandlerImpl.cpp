#include "CreateChannelInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateChannelInvitationCommandHandlerImpl::CreateChannelInvitationCommandHandlerImpl(
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : channelInvitationRepository{std::move(channelInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      channelRepository{std::move(channelRepositoryInit)}
{
}

void CreateChannelInvitationCommandHandlerImpl::execute(
    const CreateChannelInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating channel invitation... {{senderId: {}, recipient: {},channelId: {}}}",
                               payload.senderId, payload.recipientId, payload.channelId);

    const auto sender = userRepository->findUserById({payload.senderId});

    if (!sender)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.senderId)};
    }

    const auto recipient = userRepository->findUserById({payload.recipientId});

    if (!recipient)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.recipientId)};
    }

    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{std::format("Channel with id {} not found.", payload.channelId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto channelInvitationId = uuid.str();

    const auto channelInvitation =
        channelInvitationRepository->createChannelInvitation({channelInvitationId, *sender, *recipient, *channel});

    LOG_S(INFO) << std::format("Channel invitation created. {{channelInvitationId: {}}}", channelInvitation.getId());
}
}
