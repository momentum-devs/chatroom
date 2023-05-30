#pragma once

#include <string>

namespace common::utils
{
class EnvironmentParser
{
public:
    static std::string parseString(const std::string& envName);
    static int parseInt(const std::string& envName);
};
}
