#pragma once

#include <memory>

#include "Channel.h"
#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"
#include "server/domain/entities/userChannel/UserChannel.h"
#include "User.h"
#include "UserChannel.h"

namespace server::tests
{
class UserChannelTestFactory
{
public:
    static std::shared_ptr<domain::UserChannel>
    createDomainUserChannel(const std::shared_ptr<domain::User>& user, const std::shared_ptr<domain::Channel>& channel);
    static std::shared_ptr<infrastructure::UserChannel>
    createPersistentUserChannel(const std::shared_ptr<infrastructure::User>& user,
                                const std::shared_ptr<infrastructure::Channel>& channel);
};
}
