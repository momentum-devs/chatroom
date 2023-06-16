#pragma once

#include "TokenService.h"

namespace server::application
{
class TokenServiceImpl : public TokenService
{
public:
    TokenServiceImpl(std::string jwtSecret, unsigned jwtExpiresIn);

    std::string createToken(const std::string& userId) const override;
    std::string getUserIdFromToken(const std::string& token) const override;

private:
    std::string jwtSecret;
    unsigned jwtExpiresIn;
};
}
