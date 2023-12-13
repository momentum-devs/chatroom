#pragma once

#include <odb/sqlite/database.hxx>

#include "DatabaseConfig.h"

namespace server::core
{
class DatabaseConnectionFactory
{
public:
    static std::shared_ptr<odb::sqlite::database> create(const DatabaseConfig& config);
};
}
