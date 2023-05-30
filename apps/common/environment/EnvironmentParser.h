#pragma once

#include <string>

namespace common::environment
{
class EnvironmentParser
{
public:
    static std::string parseString(const std::string& envName);
    static int parseInt(const std::string& envName);
};
}
