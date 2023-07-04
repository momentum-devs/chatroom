#pragma once

#include <string>

namespace server::domain
{
class User
{
public:
    User(std::string id, std::string email, std::string password, std::string nickname, bool active,
         std::string createdAt, std::string updatedAt);

    std::string getId() const;
    std::string getEmail() const;
    std::string getPassword() const;
    std::string getNickname() const;
    bool isActive() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;
    void setPassword(const std::string& newPassword);
    void setNickname(const std::string& newNickname);
    bool operator==(const User&) const;

private:
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
    bool active;
    std::string createdAt;
    std::string updatedAt;
};
}
