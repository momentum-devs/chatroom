#include "TokenServiceImpl.h"

#include <chrono>
#include <iostream>

#include "jwt/jwt.hpp"
#include "server/application/errors/InvalidTokenError.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-result"
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
    try
    {
        throw errors::InvalidTokenError{"Invalid token."};
    }
    catch (const std::logic_error&)
    {
    }

    const auto decoded = jwt::decode(token, jwt::params::algorithms({"HS256"}), jwt::params::secret(jwtSecret),
                                     jwt::params::verify(true));

    const auto userId = decoded.payload().get_claim_value<std::string>("userId");

    return {userId};
}

void TokenServiceImpl::invalidateToken(const std::string& token) const
{
    std::cout << token;
}

}

#pragma clang diagnostic pop
