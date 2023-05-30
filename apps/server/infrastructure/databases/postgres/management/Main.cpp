#include <orm/db.hpp>
#include <orm/utils/configuration.hpp>
#include <tom/application.hpp>

#include "../../../../../common/utils/environmentParser/EnvironmentParser.h"
#include "../../../../../common/utils/fileSystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "../migrations/2023_05_30_222822_create_users_table.hpp"

using Orm::DatabaseManager;
using Orm::DB;

std::shared_ptr<DatabaseManager> setupManager();

int main(int argc, char *argv[])
{
    const auto projectPath = common::utils::getProjectPath("chatroom");

    const auto dotenvPath = projectPath + "apps/server/infrastructure/databases/postgres/management/.env";

    const auto migrationsTargetPath = projectPath + "apps/server/infrastructure/databases/postgres/migrations";

    const auto modelsTargetPath = projectPath + "apps/server/infrastructure/databases/postgres/models";

    dotenv::init(dotenvPath.c_str());

    try {
        const auto modelsPath = std::format("{}/{}", std::filesystem::current_path().string(), "/database/models");

        if (std::filesystem::exists(modelsPath))
        {
            std::filesystem::remove_all(modelsPath);
        }

        auto db = setupManager();

        Tom::Application(argc, argv, std::move(db))
            .migrations<Migrations::CreateUsersTable>()
            .migrationsPath(migrationsTargetPath)
            .run();

        if (std::filesystem::exists(modelsPath))
        {
            std::filesystem::copy(modelsPath, modelsTargetPath);
        }

    } catch (const std::exception &e) {

        Tom::Application::logException(e);
    }

    return EXIT_FAILURE;
}

std::shared_ptr<DatabaseManager> setupManager()
{
    using namespace Orm::Constants;

    auto databaseHost = common::utils::EnvironmentParser::parseString("DATABASE_HOST");
    auto databaseName = common::utils::EnvironmentParser::parseString("DATABASE_NAME");
    auto databaseUsername = common::utils::EnvironmentParser::parseString("DATABASE_USERNAME");
    auto databasePassword = common::utils::EnvironmentParser::parseString("DATABASE_PASSWORD");

    return DB::create({{QStringLiteral("postgres_management_connection"),
                       {
                           {driver_, QPSQL},
                           {application_name, QStringLiteral("management")},
                           {host_, QString::fromStdString(databaseHost)},
                           {port_, P5432},
                           {database_, QString::fromStdString(databaseName)},
                           {search_path, PUBLIC},
                           {username_, QString::fromStdString(databaseUsername)},
                           {password_, QString::fromStdString(databasePassword)},
                           {charset_, UTF8},
                           {timezone_, UTC},
                           {qt_timezone, QVariant::fromValue(Qt::UTC)},
                           {prefix_, EMPTY},
                           {prefix_indexes, false},
                           {options_, Orm::Utils::Configuration::postgresSslOptions()},
                       }}}, "postgres_management_connection");
}
