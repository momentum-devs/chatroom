#include <orm/utils/configuration.hpp>
#include <tom/application.hpp>

#include "../../../../../common/environment/EnvironmentParser.h"
#include "../../../../../common/filesystem/GetProjectPath.h"
#include "../migrations/2023_05_30_222822_create_users_table.hpp"
#include "laserpants/dotenv/dotenv.h"
#include "DatabaseManagerFactory.h"

int main(int argc, char *argv[])
{
    const auto projectPath = common::filesystem::getProjectPath("chatroom");

    const auto postgresPath = std::format("{}/apps/server/infrastructure/databases/postgres", projectPath);

    const auto migrationsTargetPath = std::format("{}/migrations", postgresPath);

    const auto modelsTargetPath = std::format("{}/models", postgresPath);

    dotenv::init(std::format("{}/management/.env", postgresPath).c_str());

    const auto databaseHost = common::environment::EnvironmentParser::parseString("DATABASE_HOST");
    const auto databaseName = common::environment::EnvironmentParser::parseString("DATABASE_NAME");
    const auto databaseUsername = common::environment::EnvironmentParser::parseString("DATABASE_USERNAME");
    const auto databasePassword = common::environment::EnvironmentParser::parseString("DATABASE_PASSWORD");

    try {
        const auto modelsPath = std::format("{}/{}", std::filesystem::current_path().string(), "/database/models");

        if (std::filesystem::exists(modelsPath))
        {
            std::filesystem::remove_all(modelsPath);
        }

        auto databaseManager = server::infrastructure::DatabaseManagerFactory::create({databaseHost, databaseName, databaseUsername, databasePassword});

        Tom::Application(argc, argv, std::move(databaseManager))
            .migrations<Migrations::CreateUsersTable>()
            .migrationsPath(migrationsTargetPath)
            .run();

        if (std::filesystem::exists(modelsPath))
        {
            std::filesystem::copy(modelsPath, modelsTargetPath, std::filesystem::copy_options::overwrite_existing);
        }

    } catch (const std::exception &e) {

        Tom::Application::logException(e);
    }

    return EXIT_FAILURE;
}
