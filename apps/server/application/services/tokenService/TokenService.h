#pragma once

#include <map>
#include <string>

namespace server::application
{
class TokenService
{
public:
    virtual ~TokenService() = default;

    virtual std::string createToken(unsigned userId) const = 0;
    virtual unsigned getUserIdFromToken(const std::string& token) const = 0;
};
}
