#pragma once

#include <string>

namespace server::config
{
class ConfigProvider
{
public:
    std::string getDatabasePath() const;
    int getServerPort() const;
    std::string getJwtSecret() const;
    int getJwtExpireIn() const;
    std::string getSendGridApiKey() const;
    std::string getSendGridEmail() const;
    int getRefreshSessionsInterval() const;
};
}
