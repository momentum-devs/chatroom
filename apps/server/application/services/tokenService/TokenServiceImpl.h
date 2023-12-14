#pragma once

#include <memory>

#include "server/domain/repositories/blacklistTokenRepository/BlacklistTokenRepository.h"
#include "TokenService.h"

namespace server::application
{
class TokenServiceImpl : public TokenService
{
public:
    TokenServiceImpl(std::string jwtSecret, unsigned jwtExpiresIn,
                     std::shared_ptr<domain::BlacklistTokenRepository> blacklistTokenRepository);

    std::string createToken(const std::string& userId) const override;
    VerifyTokenResult verifyToken(const std::string& token) const override;
    void invalidateToken(const std::string& token) const override;

private:
    std::string jwtSecret;
    unsigned jwtExpiresIn;
    std::shared_ptr<domain::BlacklistTokenRepository> blacklistTokenRepository;
};
}
