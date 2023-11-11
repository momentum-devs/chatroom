#include "MessageTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"

namespace server::tests
{

std::shared_ptr<domain::Message>
MessageTestFactory::createDomainMessage(const std::shared_ptr<domain::User>& sender,
                                        const std::shared_ptr<domain::Channel>& channel,
                                        const std::shared_ptr<domain::Group>& group)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto content = faker::Word::words();

    return std::make_shared<domain::Message>(id, content, sender, channel, group, createdAt, updatedAt);
}

std::shared_ptr<infrastructure::Message>
MessageTestFactory::createPersistentMessage(const std::shared_ptr<infrastructure::User>& sender,
                                            const std::shared_ptr<infrastructure::Channel>& channel,
                                            const std::shared_ptr<infrastructure::Group>& group)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto content = faker::Word::words();

    return std::make_shared<infrastructure::Message>(id, content, sender, channel, group, createdAt, updatedAt);
}
}
