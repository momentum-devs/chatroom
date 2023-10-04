#pragma once

#include <memory>

#include "Channel.h"
#include "Conversation.h"
#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/conversation/Conversation.h"
#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class ConversationTestFactory
{
public:
    static std::shared_ptr<domain::Conversation>
    createDomainUsersConversation(const std::shared_ptr<domain::User>& user,
                                  const std::shared_ptr<domain::User>& recipient);
    static std::shared_ptr<domain::Conversation>
    createDomainChannelConversation(const std::shared_ptr<domain::Channel>& channel);
    static std::shared_ptr<infrastructure::Conversation>
    createPersistentUsersConversation(const std::shared_ptr<infrastructure::User>& user,
                                      const std::shared_ptr<infrastructure::User>& recipient);
    static std::shared_ptr<infrastructure::Conversation>
    createPersistentChannelConversation(const std::shared_ptr<infrastructure::Channel>& channel);
};
}
