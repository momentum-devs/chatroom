#include "ConfigProvider.h"

#include "envParser/EnvironmentParser.h"

namespace client::config
{
std::string ConfigProvider::getServerHost() const
{
    return common::envParser::EnvironmentParser::parseString("SERVER_HOST");
}

int ConfigProvider::getServerPort() const
{
    return common::envParser::EnvironmentParser::parseInt("SERVER_PORT");
}
}
