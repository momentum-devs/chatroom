#pragma once

#include <memory>

#include "Message.h"
#include "Reaction.h"
#include "server/domain/entities/message/Message.h"
#include "server/domain/entities/reaction/Reaction.h"
#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class ReactionTestFactory
{
public:
    std::shared_ptr<domain::Reaction> createDomainReaction(const std::shared_ptr<domain::User>& user,
                                                           const std::shared_ptr<domain::Message>& message);
    std::shared_ptr<infrastructure::Reaction>
    createPersistentReaction(const std::shared_ptr<infrastructure::User>& user,
                             const std::shared_ptr<infrastructure::Message>& message);
};
}
