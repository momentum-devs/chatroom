#include "CreateFriendInvitationCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateFriendInvitationCommandHandlerImpl::CreateFriendInvitationCommandHandlerImpl(
    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : friendInvitationRepository{std::move(friendInvitationRepositoryInit)},
      userRepository{std::move(userRepositoryInit)}
{
}

void CreateFriendInvitationCommandHandlerImpl::execute(const CreateFriendInvitationCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Creating friend invitation... {{senderId: {}, recipientId: {}}}", payload.senderId,
                               payload.recipientId);

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

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto friendInvitationId = uuid.str();

    const auto friendInvitation =
        friendInvitationRepository->createFriendInvitation({friendInvitationId, *sender, *recipient});

    LOG_S(INFO) << std::format("Friend invitation created. {{friendInvitationId: {}}}", friendInvitation.getId());
}
}
