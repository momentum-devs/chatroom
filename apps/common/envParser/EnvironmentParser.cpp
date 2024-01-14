#include "EnvironmentParser.h"

#include "errors/MissingEnvironmentVariableError.h"
#include "fmt/format.h"

namespace common::envParser
{
std::optional<std::string> EnvironmentParser::parseString(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        return std::nullopt;
    }

    return envValue;
}

std::string EnvironmentParser::parseRequiredString(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw errors::MissingEnvironmentVariableError(fmt::format("Error parsing environment variable {}.", envName));
    }

    return envValue;
}

int EnvironmentParser::parseRequiredInt(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw errors::MissingEnvironmentVariableError(fmt::format("Error parsing environment variable {}.", envName));
    }

    return std::stoi(envValue);
}

}
