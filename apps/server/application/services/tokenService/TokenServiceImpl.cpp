#include "TokenServiceImpl.h"

#include <chrono>

#include "jwt/jwt.hpp"

namespace server::application
{
TokenServiceImpl::TokenServiceImpl(std::string jwtSecretInit, unsigned jwtExpiresInInit)
    : jwtSecret{std::move(jwtSecretInit)}, jwtExpiresIn{jwtExpiresInInit}
{
}

std::string TokenServiceImpl::createToken(const std::string& userId) const
{
    jwt::jwt_object jwtObject{jwt::params::algorithm("HS256"), jwt::params::secret(jwtSecret)};

    jwtObject.add_claim("userId", userId)
        .add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{jwtExpiresIn});

    return jwtObject.signature();
}

VerifyTokenResult TokenServiceImpl::verifyToken(const std::string& token) const
{
    const auto decoded = jwt::decode(token, jwt::params::algorithms({"HS256"}), jwt::params::secret(jwtSecret),
                                     jwt::params::verify(true));

    const auto userId = decoded.payload().get_claim_value<std::string>("userId");

    return {userId};
}

void TokenServiceImpl::invalidateToken(const std::string& token) const
{
    cpp_redis::client client;

    client.connect();
}

}
