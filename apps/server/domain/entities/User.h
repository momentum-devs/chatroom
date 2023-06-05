#pragma once

#include <string>

namespace server::domain
{
class User
{
public:
    User(unsigned id, std::string email, std::string password, std::string nickname);

    unsigned getId() const;
    std::string getEmail() const;
    std::string getPassword() const;
    std::string getNickname() const;
    void setPassword(const std::string& newPassword);
    void setNickname(const std::string& newNickname);

private:
    unsigned id;
    std::string email;
    std::string password;
    std::string nickname;
};
}
