#include "TokenServiceImpl.h"

#include <chrono>
#include <redis-cpp/execute.h>
#include <redis-cpp/stream.h>

#include "jwt/jwt.hpp"
#include "server/application/errors/InvalidTokenError.h"

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
    auto stream = rediscpp::make_stream("localhost", "6379");

    const auto response = rediscpp::execute(*stream, "get", token);

    try
    {
        // Test if value exists in invalid tokens, if yes throws
        response.as_string();

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
    auto stream = rediscpp::make_stream("localhost", "6379");

    rediscpp::execute(*stream, "set", token, "invalid");
}

}
