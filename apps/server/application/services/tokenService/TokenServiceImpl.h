#pragma once

#include "TokenService.h"

namespace server::application
{
class TokenServiceImpl : public TokenService
{
public:
    std::string createToken(const std::map<std::string, std::string>& data) const override;
};
}
