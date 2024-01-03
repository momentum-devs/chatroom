#include <gmock/gmock.h>

#include "core/database/DatabaseMigrationsRunner.h"
#include "filesystem/GetProjectPath.h"
#include <fmt/format.h>
#include "laserpants/dotenv/dotenv.h"
#include "server/config/ConfigProvider.h"

int main(int argc, char** argv)
{
    const auto projectPath = common::filesystem::getProjectPath("chatroom");

    const auto serverRootPath = fmt::format("{}/apps/server", projectPath);

    dotenv::init(fmt::format("{}/.env", serverRootPath).c_str());

    server::config::ConfigProvider configProvider;

    const auto databaseRelativePath = configProvider.getDatabasePath();

    const auto databaseFullPath = fmt::format("{}/{}", serverRootPath, databaseRelativePath);

    const auto databaseMigrationsFilePath = fmt::format("{}/scripts/migrations.sql", serverRootPath);

    server::core::DatabaseMigrationsRunner::runMigrations(databaseFullPath, databaseMigrationsFilePath);

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
