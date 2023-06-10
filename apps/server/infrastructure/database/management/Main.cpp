#include <orm/utils/configuration.hpp>
#include <tom/application.hpp>

#include "../../../config/ConfigProvider.h"
#include "../migrations/2023_05_30_222822_create_users_table.hpp"
#include "../migrations/2023_06_10_182014_create_channels_table.hpp"
#include "DatabaseManagerFactory.h"
#include "filesystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"

int main(int argc, char* argv[])
{
    const auto projectPath = common::filesystem::getProjectPath("chatroom");

    const auto databasePath = std::format("{}/apps/server/infrastructure/database", projectPath);

    const auto migrationsTargetPath = std::format("{}/migrations", databasePath);

    dotenv::init(std::format("{}/management/.env", databasePath).c_str());

    server::config::ConfigProvider configProvider;

    const auto databaseHost = configProvider.getDatabaseHost();
    const auto databaseName = configProvider.getDatabaseName();
    const auto databaseUsername = configProvider.getDatabaseUsername();
    const auto databasePassword = configProvider.getDatabasePassword();

    try
    {
        auto databaseManager = server::infrastructure::DatabaseManagerFactory::create(
            {databaseHost, databaseName, databaseUsername, databasePassword});

        Tom::Application(argc, argv, std::move(databaseManager))
            .migrations<Migrations::CreateUsersTable, Migrations::CreateChannelsTable>()
            .migrationsPath(migrationsTargetPath)
            .run();
    }
    catch (const std::exception& e)
    {
        Tom::Application::logException(e);

        return EXIT_FAILURE;
    }
}
