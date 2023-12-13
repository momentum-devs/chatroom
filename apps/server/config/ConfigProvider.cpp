#include "ConfigProvider.h"

#include "envParser/EnvironmentParser.h"

namespace server::config
{
std::string ConfigProvider::getDatabasePath() const
{
    return common::envParser::EnvironmentParser::parseString("DATABASE_PATH");
}

int ConfigProvider::getServerPort() const
{
    return common::envParser::EnvironmentParser::parseInt("SERVER_PORT");
}

std::string ConfigProvider::getJwtSecret() const
{
    return common::envParser::EnvironmentParser::parseString("JWT_SECRET");
}

int ConfigProvider::getJwtExpireIn() const
{
    return common::envParser::EnvironmentParser::parseInt("JWT_EXPIRES_IN");
}

std::string ConfigProvider::getSendGridApiKey() const
{
    return common::envParser::EnvironmentParser::parseString("SENDGRID_API_KEY");
}

std::string ConfigProvider::getSendGridEmail() const
{
    return common::envParser::EnvironmentParser::parseString("SENDGRID_EMAIL");
}

int ConfigProvider::getRefreshSessionsInterval() const
{
    return common::envParser::EnvironmentParser::parseInt("REFRESH_SESSION_INTERVAL");
}

}
