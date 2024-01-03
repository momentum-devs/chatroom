#include "TokenServiceImpl.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include "fmt/format.h"
#include <sstream>

#include "jwt/jwt.hpp"
#include "server/application/errors/InvalidTokenError.h"

namespace server::application
{
TokenServiceImpl::TokenServiceImpl(std::string jwtSecretInit, unsigned jwtExpiresInInit,
                                   std::shared_ptr<domain::BlacklistTokenRepository> blacklistTokenRepositoryInit)
    : jwtSecret{std::move(jwtSecretInit)},
      jwtExpiresIn{jwtExpiresInInit},
      blacklistTokenRepository{std::move(blacklistTokenRepositoryInit)}
{
}

std::string TokenServiceImpl::createToken(const std::string& userId) const
{
    jwt::jwt_object jwtObject{jwt::params::algorithm("HS256"), jwt::params::secret(jwtSecret)};

    const auto expiresAt =
        (std::chrono::system_clock::now() + std::chrono::seconds{jwtExpiresIn}).time_since_epoch().count();

    jwtObject.add_claim("userId", userId).add_claim("exp", expiresAt);

    return jwtObject.signature();
}

VerifyTokenResult TokenServiceImpl::verifyToken(const std::string& token) const
{
    const auto blacklistToken = blacklistTokenRepository->findBlacklistTokenByToken({token});

    if (blacklistToken)
    {
        throw errors::InvalidTokenError{fmt::format("Invalid token `{}`", token)};
    }

    const auto decoded = jwt::decode(token, jwt::params::algorithms({"HS256"}), jwt::params::secret(jwtSecret),
                                     jwt::params::verify(true));

    const auto userId = decoded.payload().get_claim_value<std::string>("userId");

    return {userId};
}

void TokenServiceImpl::invalidateToken(const std::string& token) const
{
    const auto decoded = jwt::decode(token, jwt::params::algorithms({"HS256"}), jwt::params::secret(jwtSecret),
                                     jwt::params::verify(true));

    const auto expiresAt = decoded.payload().get_claim_value<int>("exp");

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto blacklistTokenId = uuid.str();

    blacklistTokenRepository->createBlacklistToken({blacklistTokenId, token, std::to_string(expiresAt)});
}
}
