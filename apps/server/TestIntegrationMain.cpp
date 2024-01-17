#include <aws/core/Aws.h>
#include <gmock/gmock.h>

#include "core/database/DatabaseMigrationsRunner.h"
#include "filesystem/GetProjectPath.h"
#include "fmt/format.h"
#include "server/core/configProvider/ConfigProvider.h"

int main(int argc, char** argv)
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    const auto projectPath = common::filesystem::getProjectPath("chatroom");

    const auto serverRootPath = fmt::format("{}/apps/server", projectPath);

    server::core::ConfigProvider configProvider;

    const auto databaseRelativePath = configProvider.getDatabasePath();

    const auto databaseFullPath = fmt::format("{}/{}", serverRootPath, databaseRelativePath);

    const auto databaseMigrationsFilePath = fmt::format("{}/scripts/migrations.sql", serverRootPath);

    server::core::DatabaseMigrationsRunner::runMigrations(databaseFullPath, databaseMigrationsFilePath);

    ::testing::InitGoogleMock(&argc, argv);

    const auto result = RUN_ALL_TESTS();

    Aws::ShutdownAPI(options);

    return result;
}
