#include "EnvironmentParser.h"

#include <format>

#include "errors/MissingEnvironmentVariableError.h"

namespace client::config
{
std::string EnvironmentParser::parseString(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw errors::MissingEnvironmentVariableError(std::format("Error parsing environment variable {}.", envName));
    }

    return envValue;
}

int EnvironmentParser::parseInt(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw errors::MissingEnvironmentVariableError(std::format("Error parsing environment variable {}.", envName));
    }

    return std::stoi(envValue);
}
}
