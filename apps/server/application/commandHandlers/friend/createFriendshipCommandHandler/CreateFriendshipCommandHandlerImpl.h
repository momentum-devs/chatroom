#pragma once

#include <memory>

#include "CreateFriendshipCommandHandler.h"
#include "server/domain/repositories/conversationRepository/ConversationRepository.h"
#include "server/domain/repositories/friendshipRepository/FriendshipRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateFriendshipCommandHandlerImpl : public CreateFriendshipCommandHandler
{
public:
    CreateFriendshipCommandHandlerImpl(std::shared_ptr<domain::FriendshipRepository>,
                                       std::shared_ptr<domain::UserRepository>,
                                       std::shared_ptr<domain::ConversationRepository>);

    void execute(const CreateFriendshipCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::FriendshipRepository> friendshipRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::ConversationRepository> conversationRepository;
};
}
