#pragma once

#include <memory>
#include <string>

#include "config-cxx/Config.h"

namespace server::core
{
class ConfigProvider
{
public:
    ConfigProvider();

    std::string getDatabasePath() const;
    int getServerPort() const;
    std::string getJwtSecret() const;
    int getJwtExpiresIn() const;
    std::string getSendGridApiKey() const;
    std::string getSendGridFromEmail() const;
    int getSessionRefreshInterval() const;
    std::string getCxxEnv() const;

private:
    std::unique_ptr<config::Config> config;
};
}
