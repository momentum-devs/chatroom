#pragma once

#include <memory>

#include "Channel.h"
#include "Message.h"
#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/message/Message.h"
#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class MessageTestFactory
{
public:
    std::shared_ptr<domain::Message> createDomainMessage(const std::shared_ptr<domain::User>& sender,
                                                         const std::shared_ptr<domain::Channel>& channel,
                                                         const std::shared_ptr<domain::Group>& group);
    std::shared_ptr<infrastructure::Message>
    createPersistentMessage(const std::shared_ptr<infrastructure::User>& sender,
                            const std::shared_ptr<infrastructure::Channel>& channel,
                            const std::shared_ptr<infrastructure::Group>& group);
};
}
