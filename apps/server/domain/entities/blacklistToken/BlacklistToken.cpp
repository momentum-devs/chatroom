#include "BlacklistToken.h"

#include <utility>

namespace server::domain
{
BlacklistToken::BlacklistToken(std::string idInit, std::string tokenInit, std::string expiresAtInit)
    : id{std::move(idInit)}, token{std::move(tokenInit)}, expiresAt{std::move(expiresAtInit)}
{
}

std::string BlacklistToken::getId() const
{
    return id;
}

std::string BlacklistToken::getToken() const
{
    return token;
}

std::string BlacklistToken::getExpiresAt() const
{
    return expiresAt;
}

}
