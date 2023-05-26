#pragma once

#include <string>

namespace common::utils
{
class EnvironmentParser
{
public:
    std::string parseString(const std::string& envName);
    int parseInt(const std::string& envName);
};
}
