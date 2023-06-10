#pragma once

#include "TokenService.h"

namespace server::application
{
class TokenServiceImpl : public TokenService
{
public:
    explicit TokenServiceImpl(std::string jwtSecret);

    std::string createToken(const std::map<std::string, std::string>& data) const override;
    std::map<std::string, std::string> verifyToken(const std::string& token) const override;

private:
    std::string jwtSecret;
};
}
