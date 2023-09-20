#pragma once

#include <memory>

#include "DeleteFriendshipCommandHandler.h"
#include "server/domain/repositories/friendshipRepository/FriendshipRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class DeleteFriendshipCommandHandlerImpl : public DeleteFriendshipCommandHandler
{
public:
    DeleteFriendshipCommandHandlerImpl(std::shared_ptr<domain::FriendshipRepository>,
                                       std::shared_ptr<domain::UserRepository>);

    void execute(const DeleteFriendshipCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendshipRepository> friendshipRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
