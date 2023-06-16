#pragma once

#include <string>

namespace server::domain
{
class User
{
public:
    User(std::string id, std::string email, std::string password, std::string nickname);

    std::string getId() const;
    std::string getEmail() const;
    std::string getPassword() const;
    std::string getNickname() const;
    void setPassword(const std::string& newPassword);
    void setNickname(const std::string& newNickname);

private:
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
};
}
