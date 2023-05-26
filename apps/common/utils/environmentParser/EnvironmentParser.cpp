#include "EnvironmentParser.h"

#include <format>

#include "common/utils/environmentParser/errors/MissingEnvironmentVariableError.h"

namespace common::utils
{

std::string EnvironmentParser::parseString(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw MissingEnvironmentVariableError(std::format("Error parsing env variable {}", envName));
    }

    return envValue;
}

int EnvironmentParser::parseInt(const std::string& envName)
{
    auto envValue = std::getenv(envName.c_str());

    if (!envValue)
    {
        throw MissingEnvironmentVariableError(std::format("Error parsing env variable {}", envName));
    }

    return std::stoi(envValue);
}
}