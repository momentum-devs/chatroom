#include "TokenServiceImpl.h"

#include <jwt-cpp/jwt.h>
#include <utility>

#include "jwt-cpp/traits/nlohmann-json/traits.h"

namespace server::application
{
TokenServiceImpl::TokenServiceImpl(std::string jwtSecretInit) : jwtSecret{std::move(jwtSecretInit)} {}

std::string TokenServiceImpl::createToken(const std::map<std::string, std::string>& data) const
{
    using traits = jwt::traits::nlohmann_json;
    using claim = jwt::basic_claim<traits>;

    auto tokenBuilder = jwt::create<traits>().set_issuer("auth0").set_type("JWT");

    for (const auto& [key, value] : data)
    {
        tokenBuilder.set_payload_claim(key, claim(value));
    }

    return tokenBuilder.sign(jwt::algorithm::hs256{jwtSecret});
}

std::map<std::string, std::string> TokenServiceImpl::verifyToken(const std::string& token) const
{
    using traits = jwt::traits::nlohmann_json;

    auto decodedToken = jwt::decode<traits>(token);

    jwt::verify<traits>().allow_algorithm(jwt::algorithm::hs256{"secret"}).with_issuer("auth0").verify(decodedToken);

    std::map<std::string, std::string> tokenPayload;

    for (const auto& claim : decodedToken.get_payload_json())
    {
        tokenPayload.insert({claim.first, claim.second});
    }

    return tokenPayload;
}
}
