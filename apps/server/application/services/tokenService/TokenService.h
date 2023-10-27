#pragma once

#include <map>
#include <string>

#include "server/application/services/tokenService/payloads/VerifyTokenResult.h"

namespace server::application
{
class TokenService
{
public:
    virtual ~TokenService() = default;

    virtual std::string createToken(const std::string& userId) const = 0;
    virtual VerifyTokenResult verifyToken(const std::string& token) const = 0;
    virtual void invalidateToken(const std::string& token) const = 0;
};
}
