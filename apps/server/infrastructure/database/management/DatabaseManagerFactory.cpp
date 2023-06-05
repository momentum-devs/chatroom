#include "DatabaseManagerFactory.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <orm/db.hpp>
#include <orm/utils/configuration.hpp>

namespace server::infrastructure
{
bool DatabaseManagerFactory::initialized = false;

std::shared_ptr<Orm::DatabaseManager> DatabaseManagerFactory::create(const DatabaseConfig& databaseConfig)
{
    if (initialized)
    {
        addConnection(databaseConfig);

        return nullptr;
    }

    using namespace Orm::Constants;


    const auto databaseManager = Orm::DB::create({{QStringLiteral("postgres_connection"),
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

    initialized = true;

    return databaseManager;
}

void DatabaseManagerFactory::addConnection(const DatabaseConfig& databaseConfig)
{
    using namespace Orm::Constants;

    auto uuid = QString::fromStdString(boost::uuids::to_string(boost::uuids::random_generator()()));

    Orm::DB::addConnection(
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
        },
        uuid);

    Orm::DB::setDefaultConnection(uuid);
}
}
