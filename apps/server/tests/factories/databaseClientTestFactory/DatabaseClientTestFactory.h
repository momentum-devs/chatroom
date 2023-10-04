#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

namespace server::tests
{
class DatabaseClientTestFactory
{
public:
    std::shared_ptr<odb::pgsql::database> create();
};
}
