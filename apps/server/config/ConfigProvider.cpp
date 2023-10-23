#include "ConfigProvider.h"

#include "EnvironmentParser.h"

namespace server::config
{
std::string ConfigProvider::getDatabaseHost() const
{
    return EnvironmentParser::parseString("DATABASE_HOST");
}

std::string ConfigProvider::getDatabaseName() const
{
    return EnvironmentParser::parseString("DATABASE_NAME");
}

std::string ConfigProvider::getDatabaseUsername() const
{
    return EnvironmentParser::parseString("DATABASE_USERNAME");
}

std::string ConfigProvider::getDatabasePassword() const
{
    return EnvironmentParser::parseString("DATABASE_PASSWORD");
}

int ConfigProvider::getServerPort() const
{
    return EnvironmentParser::parseInt("SERVER_PORT");
}

std::string ConfigProvider::getJwtSecret() const
{
    return EnvironmentParser::parseString("JWT_SECRET");
}

int ConfigProvider::getJwtExpireIn() const
{
    return EnvironmentParser::parseInt("JWT_EXPIRES_IN");
}

std::string ConfigProvider::getSendGridApiKey() const
{
    return EnvironmentParser::parseString("SENDGRID_API_KEY");
}

std::string ConfigProvider::getSendGridEmail() const
{
    return EnvironmentParser::parseString("SENDGRID_EMAIL");
}

int ConfigProvider::getRefreshSessionsInterval() const
{
    return EnvironmentParser::parseInt("REFRESH_SESSION_INTERVAL");
}

}
