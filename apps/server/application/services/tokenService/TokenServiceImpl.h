#pragma once

#include "TokenService.h"

namespace server::application
{
class TokenServiceImpl : public TokenService
{
public:
    explicit TokenServiceImpl(std::string jwtSecret, unsigned jwtExpiresIn);

    std::string createToken(unsigned userId) const override;
    unsigned getUserIdFromToken(const std::string& token) const override;

private:
    std::string jwtSecret;
    unsigned jwtExpiresIn;
};
}
