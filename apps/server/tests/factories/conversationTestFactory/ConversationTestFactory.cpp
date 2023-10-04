#include "ConversationTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::Conversation>
ConversationTestFactory::createDomainUsersConversation(const std::shared_ptr<domain::User>& user,
                                                       const std::shared_ptr<domain::User>& recipient)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<domain::Conversation>(id, user, recipient, nullptr, createdAt, updatedAt);
}

std::shared_ptr<domain::Conversation>
ConversationTestFactory::createDomainChannelConversation(const std::shared_ptr<domain::Channel>& channel)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<domain::Conversation>(id, nullptr, nullptr, channel, createdAt, updatedAt);
}

std::shared_ptr<infrastructure::Conversation>
ConversationTestFactory::createPersistentUsersConversation(const std::shared_ptr<infrastructure::User>& user,
                                                           const std::shared_ptr<infrastructure::User>& recipient)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    return std::make_shared<infrastructure::Conversation>(id, user, recipient, nullptr, createdAt, updatedAt);
}

std::shared_ptr<infrastructure::Conversation>
ConversationTestFactory::createPersistentChannelConversation(const std::shared_ptr<infrastructure::Channel>& channel)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    
    return std::make_shared<infrastructure::Conversation>(id, nullptr, nullptr, channel, createdAt, updatedAt);
}
}
