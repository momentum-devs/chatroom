#pragma once

#include <string>

namespace server::domain
{
class BlacklistToken
{
public:
    BlacklistToken(std::string id, std::string token, std::string expiresAt);

    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getToken() const;
    [[nodiscard]] std::string getExpiresAt() const;

private:
    std::string id;
    std::string token;
    std::string expiresAt;
};
}
