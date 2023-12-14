#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

namespace server::infrastructure
{
#pragma db object table("blacklist_tokens")
class BlacklistToken
{
public:
    BlacklistToken(std::string idInit, std::string tokenInit, std::string expiresAtInit)
        : id{std::move(idInit)}, token{std::move(tokenInit)}, expires_at{std::move(expiresAtInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::string getToken() const
    {
        return token;
    }

    [[nodiscard]] std::string getExpiresAt() const
    {
        return expires_at;
    }

private:
    BlacklistToken() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string token;
    std::string expires_at;
};
}
