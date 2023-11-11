#pragma once

#include <string>

namespace common::envParser
{
class EnvironmentParser
{
public:
    static std::string parseString(const std::string& envName);
    static int parseInt(const std::string& envName);
};
}
