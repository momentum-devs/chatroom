#pragma once

#include <string>

namespace server::infrastructure
{
struct DatabaseConfig
{
    const std::string username;
    const std::string password;
    const std::string host;
    const std::string port;
    const std::string databaseName;
};

}
