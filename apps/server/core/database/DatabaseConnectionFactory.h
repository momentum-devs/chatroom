#pragma once

#include <odb/pgsql/database.hxx>

#include "DatabaseConfig.h"

namespace server::core
{
class DatabaseConnectionFactory
{
public:
    static std::shared_ptr<odb::pgsql::database> create(const DatabaseConfig& config);
};
}
