#include "BlacklistToken.h"

#include <utility>

namespace server::domain
{
BlacklistToken::BlacklistToken(std::string idInit, std::string tokenInit, std::string createdAtInit)
    : id{std::move(idInit)}, token{std::move(tokenInit)}, createdAt{std::move(createdAtInit)}
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

std::string BlacklistToken::getCreatedAt() const
{
    return createdAt;
}

}
