#include "TokenServiceImpl.h"

#include "jwt/jwt.hpp"

namespace server::application
{
TokenServiceImpl::TokenServiceImpl(std::string jwtSecretInit) : jwtSecret{std::move(jwtSecretInit)} {}

std::string TokenServiceImpl::createToken(unsigned int userId) const
{
    jwt::jwt_object jwtObject{jwt::params::algorithm("HS256"), jwt::params::secret(jwtSecret)};

    jwtObject.add_claim("userId", userId);

    auto token = jwtObject.signature();

    return token;
}

unsigned TokenServiceImpl::getUserIdFromToken(const std::string& token) const
{
    auto decoded = jwt::decode(token, jwt::params::algorithms({"HS256"}), jwt::params::secret("secret"),
                               jwt::params::verify(true));

    return decoded.payload().get_claim_value<unsigned>("userId");
}

}
