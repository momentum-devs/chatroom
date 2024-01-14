#pragma once

#include <optional>
#include <string>

namespace common::envParser
{
class EnvironmentParser
{
public:
    static std::optional<std::string> parseString(const std::string& envName);
    static std::string parseRequiredString(const std::string& envName);
    static int parseRequiredInt(const std::string& envName);
};
}
