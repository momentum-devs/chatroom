#include "DatabaseConnectionFactory.h"

namespace server::core
{
std::shared_ptr<odb::pgsql::database> DatabaseConnectionFactory::create(const DatabaseConfig& config)
{
    return std::make_shared<odb::pgsql::database>(config.username, config.password, config.databaseName, config.host,
                                                  5432);
}
}
