#pragma once

#include <memory>

#include "FindUserFriendsQueryHandler.h"
#include "server/domain/repositories/friendshipRepository/FriendshipRepository.h"

namespace server::application
{
class FindUserFriendsQueryHandlerImpl : public FindUserFriendsQueryHandler
{
public:
    explicit FindUserFriendsQueryHandlerImpl(std::shared_ptr<domain::FriendshipRepository>);

    FindUserFriendsQueryHandlerResult execute(const FindUserFriendsQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendshipRepository> friendshipRepository;
};
}
