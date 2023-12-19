#pragma once

#include <optional>
#include <string>

namespace server::domain
{
class User
{
public:
    User(std::string id, std::string email, std::string password, std::string nickname, bool active, bool emailVerified,
         std::string verificationCode, std::string createdAt, std::string updatedAt,
         std::optional<std::string> avatarUrl);

    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getEmail() const;
    [[nodiscard]] std::string getPassword() const;
    [[nodiscard]] std::string getNickname() const;
    [[nodiscard]] bool isActive() const;
    [[nodiscard]] bool isEmailVerified() const;
    [[nodiscard]] std::string getVerificationCode() const;
    [[nodiscard]] std::string getCreatedAt() const;
    [[nodiscard]] std::string getUpdatedAt() const;
    [[nodiscard]] std::optional<std::string> getAvatarUrl() const;

    void setPassword(const std::string& newPassword);
    void setNickname(const std::string& newNickname);
    void setActive(bool);
    void setEmailVerified(bool);
    void setVerificationCode(const std::string& verificationCode);
    void setAvatarUrl(const std::string& avatarUrl);
    bool operator==(const User&) const;

private:
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
    bool active;
    bool emailVerified;
    std::string verificationCode;
    std::string createdAt;
    std::string updatedAt;
    std::optional<std::string> avatarUrl;
};
}
