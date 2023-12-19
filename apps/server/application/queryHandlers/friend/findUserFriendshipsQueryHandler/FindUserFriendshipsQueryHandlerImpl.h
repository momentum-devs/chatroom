#pragma once

#include <memory>

#include "FindUserFriendshipsQueryHandler.h"
#include "server/domain/repositories/friendshipRepository/FriendshipRepository.h"

namespace server::application
{
class FindUserFriendshipsQueryHandlerImpl : public FindUserFriendshipsQueryHandler
{
public:
    explicit FindUserFriendshipsQueryHandlerImpl(std::shared_ptr<domain::FriendshipRepository>);

    FindUserFriendshipsQueryHandlerResult execute(const FindUserFriendshipsQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendshipRepository> friendshipRepository;
};
}
