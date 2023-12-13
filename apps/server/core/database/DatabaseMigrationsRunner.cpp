#include "DatabaseMigrationsRunner.h"

#include <fstream>
#include <iostream>
#include <sqlite3.h>
#include <sstream>

namespace server::core
{
namespace
{
void executeSqlScript(sqlite3* db, const std::string& sqlScript)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sqlScript.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error executing SQL script: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "SQL script executed successfully." << std::endl;
    }
}
}

void DatabaseMigrationsRunner::runMigrations(const std::string& databasePath, const std::string& sqlScriptPath)
{
    sqlite3* db;
    int rc = sqlite3_open(databasePath.c_str(), &db);

    if (rc == SQLITE_OK)
    {
        std::ifstream sqlScriptFile(sqlScriptPath);

        if (sqlScriptFile.is_open())
        {
            std::ostringstream buffer;
            buffer << sqlScriptFile.rdbuf();
            std::string sqlScript = buffer.str();

            executeSqlScript(db, sqlScript);

            sqlScriptFile.close();
        }
        else
        {
            std::cerr << "Error opening SQL file: " << sqlScriptPath << std::endl;
        }

        sqlite3_close(db);
    }
    else
    {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
}
}
