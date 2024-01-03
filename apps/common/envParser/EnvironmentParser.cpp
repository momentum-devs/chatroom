#include "EnvironmentParser.h"

#include "errors/MissingEnvironmentVariableError.h"
#include <fmt/format.h>

namespace common::envParser
{
std::string EnvironmentParser::parseString(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw errors::MissingEnvironmentVariableError(fmt::format("Error parsing environment variable {}.", envName));
    }

    return envValue;
}

int EnvironmentParser::parseInt(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw errors::MissingEnvironmentVariableError(fmt::format("Error parsing environment variable {}.", envName));
    }

    return std::stoi(envValue);
}
}
