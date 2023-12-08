#include "ReactionTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"

namespace server::tests
{
std::shared_ptr<domain::Reaction>
ReactionTestFactory::createDomainReaction(const std::shared_ptr<domain::User>& user,
                                          const std::shared_ptr<domain::Message>& message)
{
    const auto id = faker::String::uuid();
    const auto name = faker::Word::noun();

    return std::make_shared<domain::Reaction>(id, name, user, message);
}

std::shared_ptr<infrastructure::Reaction>
ReactionTestFactory::createPersistentReaction(const std::shared_ptr<infrastructure::User>& user,
                                              const std::shared_ptr<infrastructure::Message>& message)
{
    const auto id = faker::String::uuid();
    const auto name = faker::Word::noun();

    return std::make_shared<infrastructure::Reaction>(id, name, user, message);
}
}
