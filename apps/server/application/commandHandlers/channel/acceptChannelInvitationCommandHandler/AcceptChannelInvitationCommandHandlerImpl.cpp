#include "AcceptChannelInvitationCommandHandlerImpl.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fmt/format.h>
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
AcceptChannelInvitationCommandHandlerImpl::AcceptChannelInvitationCommandHandlerImpl(
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit,
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : channelInvitationRepository{std::move(channelInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      userChannelRepository{std::move(userChannelRepositoryInit)},
      channelRepository{std::move(channelRepositoryInit)}
{
}

void AcceptChannelInvitationCommandHandlerImpl::execute(
    const AcceptChannelInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Accepting channel invitation... {{recipientId: {}, channelInvitationId: {}}}",
                               payload.recipientId, payload.channelInvitationId);

    const auto channelInvitation =
        channelInvitationRepository->findChannelInvitationById({payload.channelInvitationId});

    if (!channelInvitation)
    {
        throw errors::ResourceNotFoundError{
            fmt::format("Channel invitation with id {} not found.", payload.channelInvitationId)};
    }

    if (payload.recipientId != channelInvitation->getRecipient()->getId())
    {
        throw errors::OperationNotValidError{
            fmt::format("User with id {} is not recipient of the channel invitation with id {}.", payload.recipientId,
                        payload.channelInvitationId)};
    }

    const auto recipientId = channelInvitation->getRecipient()->getId();
    const auto channelId = channelInvitation->getChannel()->getId();

    // TODO: consider adding transactions

    addUserToChannel(recipientId, channelId);

    channelInvitationRepository->deleteChannelInvitation({*channelInvitation});

    LOG_S(INFO) << fmt::format("Channel invitation accepted. {{channelInvitationId: {}}}", channelInvitation->getId());
}

void AcceptChannelInvitationCommandHandlerImpl::addUserToChannel(const std::string& userId,
                                                                 const std::string& channelId) const
{
    const auto userChannels = userChannelRepository->findUsersChannelsByUserId({userId});

    const auto userAlreadyInChannel =
        std::any_of(userChannels.begin(), userChannels.end(), [=](const domain::UserChannel& userChannel)
                    { return userChannel.getChannel()->getId() == channelId; });

    if (userAlreadyInChannel)
    {
        throw errors::OperationNotValidError(
            fmt::format("User already is a member of channel. {{userId: {}, channelId: {}}}", userId, channelId));
    }

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
