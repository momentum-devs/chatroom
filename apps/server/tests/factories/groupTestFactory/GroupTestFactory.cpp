#include "GroupTestFactory.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"

namespace server::tests
{
std::shared_ptr<domain::Group> GroupTestFactory::createDomainGroup()
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<domain::Group>(id, createdAt);
}

std::shared_ptr<infrastructure::Group> GroupTestFactory::createPersistentGroup()
{
    const auto id = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();

    return std::make_shared<infrastructure::Group>(id, createdAt);
}

}
