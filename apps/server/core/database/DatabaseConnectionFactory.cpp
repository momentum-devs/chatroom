#include "DatabaseConnectionFactory.h"

namespace server::core
{
std::shared_ptr<odb::sqlite::database> DatabaseConnectionFactory::create(const DatabaseConfig& config)
{
    return std::make_shared<odb::sqlite::database>(config.path);
}
}
