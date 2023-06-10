#pragma once

#include <string>

namespace server::application
{
class HashService
{
public:
    virtual ~HashService() = default;

    virtual std::string hash(const std::string& data) const = 0;
    virtual bool compare(const std::string& plainData, const std::string& hashedData) const = 0;
};
}
