#include "CreateChannelInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fmt/format.h>
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateChannelInvitationCommandHandlerImpl::CreateChannelInvitationCommandHandlerImpl(
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit,
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit)
    : channelInvitationRepository{std::move(channelInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      channelRepository{std::move(channelRepositoryInit)},
      userChannelRepository{std::move(userChannelRepositoryInit)}
{
}

void CreateChannelInvitationCommandHandlerImpl::execute(
    const CreateChannelInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Creating channel invitation... {{senderId: {}, recipientId: {}, channelId: {}}}",
                               payload.senderId, payload.recipientId, payload.channelId);

    if (payload.senderId == payload.recipientId)
    {
        throw errors::OperationNotValidError{fmt::format(
            "Channel invitation's sender and recipient cannot be the same user. {{senderId: {}, recipientId: {}}}.",
            payload.senderId, payload.recipientId)};
    }

    const auto existingInvitation =
        channelInvitationRepository->findChannelInvitation({payload.senderId, payload.recipientId, payload.channelId});

    if (existingInvitation)
    {
        throw errors::OperationNotValidError{
            fmt::format("Recipient {} is already invited to channel {}.", payload.recipientId, payload.channelId)};
    }

    const auto sender = userRepository->findUserById({payload.senderId});

    if (!sender)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.senderId)};
    }

    const auto recipient = userRepository->findUserById({payload.recipientId});

    if (!recipient)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.recipientId)};
    }

    const auto channel = channelRepository->findChannelById({payload.channelId});

    if (!channel)
    {
        throw errors::ResourceNotFoundError{fmt::format("Channel with id {} not found.", payload.channelId)};
    }

    if (channel->get()->getCreator()->getId() != sender->get()->getId())
    {
        throw errors::OperationNotValidError{
            fmt::format("Sender with id {} cannot send invitation because he is not the creator of channel {}.",
                        payload.recipientId, payload.channelId)};
    }

    const auto userChannelWithRecipient =
        userChannelRepository->findUserChannel({payload.recipientId, payload.channelId});

    if (userChannelWithRecipient)
    {
        throw errors::OperationNotValidError{fmt::format("Recipient with id {} already is a member of channel {}.",
                                                         payload.recipientId, payload.channelId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto channelInvitationId = uuid.str();

    const auto channelInvitation =
        channelInvitationRepository->createChannelInvitation({channelInvitationId, *sender, *recipient, *channel});

    LOG_S(INFO) << fmt::format("Channel invitation created. {{channelInvitationId: {}}}", channelInvitation.getId());
}
}
