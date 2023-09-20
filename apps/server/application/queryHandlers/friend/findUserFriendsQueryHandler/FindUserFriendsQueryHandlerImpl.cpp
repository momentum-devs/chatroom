#include "FindUserFriendsQueryHandlerImpl.h"

namespace server::application
{
FindUserFriendsQueryHandlerImpl::FindUserFriendsQueryHandlerImpl(
    std::shared_ptr<domain::FriendshipRepository> friendshipRepositoryInit)
    : friendshipRepository{std::move(friendshipRepositoryInit)}
{
}

FindUserFriendsQueryHandlerResult
FindUserFriendsQueryHandlerImpl::execute(const FindUserFriendsQueryHandlerPayload& payload) const
{
    const auto friendships = friendshipRepository->findFriendshipsByUserId({payload.userId});

    std::vector<domain::User> friends;

    for (const auto& friendship : friendships)
    {
        if (friendship.getUser()->getId() == payload.userId)
        {
            friends.push_back(*friendship.getUserFriend());
        }
        else
        {
            friends.push_back(*friendship.getUser());
        }
    }

    return {friends};
}
}
