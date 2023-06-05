#include "ConfigProvider.h"

#include "EnvironmentParser.h"

namespace client::config
{
std::string ConfigProvider::getServerHost() const
{
    return EnvironmentParser::parseString("SERVER_HOST");
}

int ConfigProvider::getServerPort() const
{
    return EnvironmentParser::parseInt("SERVER_PORT");
}
}
