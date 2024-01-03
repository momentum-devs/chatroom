#include "FindUserFriendshipsQueryHandlerImpl.h"

namespace server::application
{
FindUserFriendshipsQueryHandlerImpl::FindUserFriendshipsQueryHandlerImpl(
    std::shared_ptr<domain::FriendshipRepository> friendshipRepositoryInit)
    : friendshipRepository{std::move(friendshipRepositoryInit)}
{
}

FindUserFriendshipsQueryHandlerResult
FindUserFriendshipsQueryHandlerImpl::execute(const FindUserFriendshipsQueryHandlerPayload& payload) const
{
    const auto friendships = friendshipRepository->findFriendshipsByUserId({payload.userId});

    return {friendships};
}
}
