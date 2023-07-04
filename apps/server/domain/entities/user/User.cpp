#include "User.h"

#include <utility>

namespace server::domain
{
User::User(std::string idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit,
           bool activeInit, bool emailVerifiedInit, std::string createdAtInit, std::string updatedAtInit)
    : id{std::move(idInit)},
      email{std::move(emailInit)},
      password{std::move(passwordInit)},
      nickname{std::move(nicknameInit)},
      active{activeInit},
      emailVerified{emailVerifiedInit},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)}
{
}

std::string User::getId() const
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

bool User::isActive() const
{
    return active;
}

bool User::isEmailVerified() const
{
    return emailVerified;
}

std::string User::getCreatedAt() const
{
    return createdAt;
}

std::string User::getUpdatedAt() const
{
    return updatedAt;
}

void User::setPassword(const std::string& newPassword)
{
    password = newPassword;
}

void User::setNickname(const std::string& newNickname)
{
    nickname = newNickname;
}

void User::setActive(bool activeInit)
{
    active = activeInit;
}

void User::setEmailVerified(bool emailVerifiedInit)
{
    emailVerified = emailVerifiedInit;
}

bool User::operator==(const User& user) const
{
    auto tieStruct = [](const User& user)
    { return std::tie(user.id, user.email, user.password, user.nickname, user.createdAt, user.updatedAt); };

    return tieStruct(*this) == tieStruct(user);
}

}
