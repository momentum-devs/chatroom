#pragma once

#include <string>

namespace server::domain
{
class BlacklistToken
{
public:
    BlacklistToken(std::string id, std::string token, std::string createdAt);

    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getToken() const;
    [[nodiscard]] std::string getCreatedAt() const;

private:
    std::string id;
    std::string token;
    std::string createdAt;
};
}
