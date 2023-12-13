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
    BlacklistToken(std::string idInit, std::string tokenInit, std::string createdAtInit)
        : id{std::move(idInit)}, token{std::move(tokenInit)}, created_at{std::move(createdAtInit)}
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

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

private:
    BlacklistToken() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string token;
    std::string created_at;
};
}
