#include "ConfigProvider.h"

#include "envParser/EnvironmentParser.h"

namespace server::config
{
std::string ConfigProvider::getDatabaseHost() const
{
    return common::envParser::EnvironmentParser::parseString("DATABASE_HOST");
}

std::string ConfigProvider::getDatabaseName() const
{
    return common::envParser::EnvironmentParser::parseString("DATABASE_NAME");
}

std::string ConfigProvider::getDatabaseUsername() const
{
    return common::envParser::EnvironmentParser::parseString("DATABASE_USERNAME");
}

std::string ConfigProvider::getDatabasePassword() const
{
    return common::envParser::EnvironmentParser::parseString("DATABASE_PASSWORD");
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
