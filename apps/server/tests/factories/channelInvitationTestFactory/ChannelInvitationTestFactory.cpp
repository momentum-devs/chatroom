#include "ChannelInvitationTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::ChannelInvitation>
ChannelInvitationTestFactory::createDomainChannelInvitation(const std::shared_ptr<domain::User>& sender,
                                                            const std::shared_ptr<domain::User>& recipient,
                                                            const std::shared_ptr<domain::Channel>& channel)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<domain::ChannelInvitation>(id, sender, recipient, channel, createdAt);
}

std::shared_ptr<infrastructure::ChannelInvitation>
ChannelInvitationTestFactory::createPersistentChannelInvitation(const std::shared_ptr<infrastructure::User>& sender,
                                                                const std::shared_ptr<infrastructure::User>& recipient,
                                                                const std::shared_ptr<infrastructure::Channel>& channel)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<infrastructure::ChannelInvitation>(id, sender, recipient, channel, createdAt);
}
}
