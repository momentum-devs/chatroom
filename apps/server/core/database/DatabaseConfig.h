#pragma once

#include <string>

namespace server::core
{
struct DatabaseConfig
{
    const std::string host;
    const std::string databaseName;
    const std::string username;
    const std::string password;
};
}
