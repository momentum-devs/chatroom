#include "DatabaseClientTestFactory.h"

namespace server::tests
{

std::shared_ptr<odb::pgsql::database> DatabaseClientTestFactory::create()
{
    std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);
}
}
