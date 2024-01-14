#include "ConfigProvider.h"

#include "envParser/EnvironmentParser.h"

namespace client::config
{
std::string ConfigProvider::getServerHost() const
{
    return common::envParser::EnvironmentParser::parseRequiredString("SERVER_HOST");
}

int ConfigProvider::getServerPort() const
{
    return common::envParser::EnvironmentParser::parseRequiredInt("SERVER_PORT");
}
}
