#include "User.h"

namespace server::domain
{
User::User(unsigned idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit)
    : id{idInit}, email{std::move(emailInit)}, password{std::move(passwordInit)}, nickname{std::move(nicknameInit)}
{
}

unsigned User::getId() const
{
    return id;
}

std::string User::getEmail() const
{
    return email;
}

std::string User::getPassword() const
{
    return password;
}

std::string User::getNickname() const
{
    return nickname;
}

void User::setPassword(const std::string& newPassword)
{
    password = newPassword;
}

void User::setNickname(const std::string& newNickname)
{
    nickname = newNickname;
}
}
