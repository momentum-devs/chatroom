#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>

namespace server::tests
{
class DatabaseClientTestFactory
{
public:
    static std::shared_ptr<odb::sqlite::database> create();
};
}
