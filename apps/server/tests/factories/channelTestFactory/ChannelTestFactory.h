#pragma once

#include <memory>
#include <optional>

#include "Channel.h"
#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class ChannelTestFactory
{
public:
    std::shared_ptr<domain::Channel> createDomainChannel(const std::shared_ptr<domain::User>& user);
    std::shared_ptr<infrastructure::Channel> createPersistentChannel(const std::shared_ptr<infrastructure::User>& user);
};
}
