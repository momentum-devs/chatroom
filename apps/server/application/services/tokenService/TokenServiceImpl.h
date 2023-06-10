#pragma once

#include "TokenService.h"

namespace server::application
{
class TokenServiceImpl : public TokenService
{
public:
    explicit TokenServiceImpl(std::string jwtSecret);

    std::string createToken(unsigned userId) const override;
    unsigned getUserIdFromToken(const std::string& token) const override;

private:
    std::string jwtSecret;
};
}
