#pragma once

#include <string>

namespace common::environment
{
class EnvironmentParser
{
public:
    std::string parseString(const std::string& envName);
    int parseInt(const std::string& envName);
};
}
