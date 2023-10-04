#pragma once

#include <memory>

#include "Channel.h"
#include "ChannelInvitation.h"
#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/channelInvitation/ChannelInvitation.h"
#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class ChannelInvitationTestFactory
{
public:
    std::shared_ptr<domain::ChannelInvitation>
    createDomainChannelInvitation(const std::shared_ptr<domain::User>& sender,
                                  const std::shared_ptr<domain::User>& recipient,
                                  const std::shared_ptr<domain::Channel>& channel);
    std::shared_ptr<infrastructure::ChannelInvitation>
    createPersistentChannelInvitation(const std::shared_ptr<infrastructure::User>& sender,
                                      const std::shared_ptr<infrastructure::User>& recipient,
                                      const std::shared_ptr<infrastructure::Channel>& channel);
};
}
