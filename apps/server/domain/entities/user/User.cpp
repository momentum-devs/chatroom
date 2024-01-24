#include "User.h"

#include <utility>

namespace server::domain
{
User::User(std::string idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit,
           bool activeInit, bool emailVerifiedInit, std::string verificationCodeInit,
           std::optional<std::string> resetPasswordCodeInit, std::string createdAtInit, std::string updatedAtInit,
           std::optional<std::string> avatarUrlInit)
    : id{std::move(idInit)},
      email{std::move(emailInit)},
      password{std::move(passwordInit)},
      nickname{std::move(nicknameInit)},
      active{activeInit},
      emailVerified{emailVerifiedInit},
      verificationCode{std::move(verificationCodeInit)},
      resetPasswordCode{std::move(resetPasswordCodeInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)},
      avatarUrl{std::move(avatarUrlInit)}
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

std::string User::getVerificationCode() const
{
    return verificationCode;
}

std::optional<std::string> User::getResetPasswordCode() const
{
    return resetPasswordCode;
}

std::string User::getCreatedAt() const
{
    return createdAt;
}

std::string User::getUpdatedAt() const
{
    return updatedAt;
}

std::optional<std::string> User::getAvatarUrl() const
{
    return avatarUrl;
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

void User::setVerificationCode(const std::string& newVerificationCode)
{
    verificationCode = newVerificationCode;
}

void User::setResetPasswordCode(const std::string& newResetPasswordCode)
{
    resetPasswordCode = newResetPasswordCode;
}

void User::setAvatarUrl(const std::string& avatarUrlInit)
{
    avatarUrl = avatarUrlInit;
}

bool User::operator==(const User& user) const
{
    auto tieStruct = [](const User& user)
    {
        return std::tie(user.id, user.email, user.password, user.nickname, user.emailVerified, user.active,
                        user.verificationCode, user.createdAt, user.updatedAt, user.resetPasswordCode);
    };

    return tieStruct(*this) == tieStruct(user);
}

}
