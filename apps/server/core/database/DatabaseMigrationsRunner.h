#pragma once

#include <string>

namespace server::core
{
class DatabaseMigrationsRunner
{
public:
    static void runMigrations(const std::string& databasePath, const std::string& sqlScriptPath);
};
}
