#include "DatabaseManagerFactory.h"

#include <orm/db.hpp>
#include <orm/utils/configuration.hpp>

namespace server::infrastructure
{
std::shared_ptr<Orm::DatabaseManager> DatabaseManagerFactory::create(const DatabaseConfig& databaseConfig)
{
    using namespace Orm::Constants;

    return Orm::DB::create({{QStringLiteral("postgres_connection"),
                             {
                                 {driver_, QPSQL},
                                 {application_name, QStringLiteral("chatroom")},
                                 {host_, QString::fromStdString(databaseConfig.host)},
                                 {port_, P5432},
                                 {database_, QString::fromStdString(databaseConfig.databaseName)},
                                 {search_path, PUBLIC},
                                 {username_, QString::fromStdString(databaseConfig.username)},
                                 {password_, QString::fromStdString(databaseConfig.password)},
                                 {charset_, UTF8},
                                 {timezone_, UTC},
                                 {qt_timezone, QVariant::fromValue(Qt::UTC)},
                                 {prefix_, EMPTY},
                                 {prefix_indexes, false},
                                 {options_, Orm::Utils::Configuration::postgresSslOptions()},
                             }}},
                           "postgres_connection");
}
}
