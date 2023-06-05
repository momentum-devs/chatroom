#pragma once

#include <map>
#include <string>

namespace server::application
{
class TokenService
{
public:
    virtual ~TokenService() = default;

    virtual std::string createToken(const std::map<std::string, std::string>& data) const = 0;
};
}
