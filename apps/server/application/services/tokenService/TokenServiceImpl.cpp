#include "TokenServiceImpl.h"

#include <jwt-cpp/jwt.h>

namespace server::application
{

std::string TokenServiceImpl::createToken(const std::map<std::string, std::string>& data) const
{
    return jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_payload_claim("sample", jwt::claim(std::string("test")))
        .sign(jwt::algorithm::hs256{"secret"});
}
}
