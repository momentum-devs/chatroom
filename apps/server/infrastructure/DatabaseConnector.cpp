#include "DatabaseConnector.h"

#include "fmt/core.h"

namespace server::infrastructure
{
DatabaseConnector::DatabaseConnector(const DatabaseConfig& databaseConfigInit) : config{databaseConfigInit} {}

std::shared_ptr<tao::pq::connection> DatabaseConnector::getConnection()
{
    if (!connection || connection->status() != tao::pq::connection_status::ok)
    {
        const auto uri = fmt::format("postgresql://{}:{}@{}:{}/{}", config.username, config.password, config.host,
                                     config.port, config.databaseName);

        connection = tao::pq::connection::create(uri);
    }

    return connection;
}
}
