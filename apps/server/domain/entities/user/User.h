#pragma once

#include <string>

namespace server::domain
{
class User
{
public:
    User(std::string id, std::string email, std::string password, std::string nickname, bool active, bool emailVerified,
         std::string createdAt, std::string updatedAt);

    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getEmail() const;
    [[nodiscard]] std::string getPassword() const;
    [[nodiscard]] std::string getNickname() const;
    [[nodiscard]] bool isActive() const;
    [[nodiscard]] bool isEmailVerified() const;
    [[nodiscard]] std::string getCreatedAt() const;
    [[nodiscard]] std::string getUpdatedAt() const;
    void setPassword(const std::string& newPassword);
    void setNickname(const std::string& newNickname);
    bool operator==(const User&) const;

private:
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
    bool active;
    bool emailVerified;
    std::string createdAt;
    std::string updatedAt;
};
}
