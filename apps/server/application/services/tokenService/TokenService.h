#pragma once

#include <map>
#include <string>

namespace server::application
{
class TokenService
{
public:
    virtual ~TokenService() = default;

    virtual std::string createToken(const std::string& userId) const = 0;
    virtual std::string getUserIdFromToken(const std::string& token) const = 0;
};
}
