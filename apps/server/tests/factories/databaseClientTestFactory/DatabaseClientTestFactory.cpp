#include "DatabaseClientTestFactory.h"

#include <iostream>

#include "filesystem/GetProjectPath.h"
#include "fmt/format.h"
#include "server/config/ConfigProvider.h"

namespace server::tests
{

std::shared_ptr<odb::sqlite::database> DatabaseClientTestFactory::create()
{
    server::config::ConfigProvider configProvider;

    const auto projectPath = common::filesystem::getProjectPath("chatroom");

    const auto serverRootPath = fmt::format("{}/apps/server", projectPath);

    const auto databaseRelativePath = configProvider.getDatabasePath();

    const auto databaseFullPath = fmt::format("{}/{}", serverRootPath, databaseRelativePath);

    return std::make_shared<odb::sqlite::database>(databaseFullPath);
}
}
