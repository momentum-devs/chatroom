#include "UserGroupTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::UserGroup>
UserGroupTestFactory::createDomainUserGroup(const std::shared_ptr<domain::User>& user,
                                            const std::shared_ptr<domain::Group>& group)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<domain::UserGroup>(id, user, group, createdAt);
}

std::shared_ptr<infrastructure::UserGroup>
UserGroupTestFactory::createPersistentUserGroup(const std::shared_ptr<infrastructure::User>& user,
                                                const std::shared_ptr<infrastructure::Group>& group)
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<infrastructure::UserGroup>(id, user, group, createdAt);
}
}
