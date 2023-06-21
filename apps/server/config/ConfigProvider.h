#pragma once

#include <string>

namespace server::config
{
class ConfigProvider
{
public:
    std::string getDatabaseHost() const;
    std::string getDatabaseName() const;
    std::string getDatabaseUsername() const;
    std::string getDatabasePassword() const;
    int getServerPort() const;
    std::string getJwtSecret() const;
    int getJwtExpireIn() const;
};
}
