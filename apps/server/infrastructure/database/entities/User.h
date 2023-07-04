#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

namespace server::infrastructure
{
#pragma db object table("users")
class User
{
public:
    User(std::string idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit, bool activeInit,
         bool emailVerifiedInit, std::string createdAtInit, std::string updatedAtInit)
        : id{std::move(idInit)},
          email{std::move(emailInit)},
          password{std::move(passwordInit)},
          nickname{std::move(nicknameInit)},
          active{activeInit},
          email_verified{emailVerifiedInit},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::string getEmail() const
    {
        return email;
    }

    [[nodiscard]] std::string getPassword() const
    {
        return password;
    }

    [[nodiscard]] std::string getNickname() const
    {
        return nickname;
    }

    [[nodiscard]] bool isActive() const
    {
        return active;
    }

    [[nodiscard]] bool isEmailVerified() const
    {
        return email_verified;
    }

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

    [[nodiscard]] std::string getUpdatedAt() const
    {
        return updated_at;
    }

    void setNickname(const std::string& nicknameInit)
    {
        nickname = nicknameInit;
    }

    void setPassword(const std::string& passwordInit)
    {
        password = passwordInit;
    }

    void setActive(bool activeInit)
    {
        active = activeInit;
    }

    void setEmailVerified(bool emailVerifiedInit)
    {
        email_verified = emailVerifiedInit;
    }

    bool operator==(const User& user) const
    {
        auto tieStruct = [](const User& user)
        { return std::tie(user.id, user.email, user.password, user.nickname, user.created_at, user.updated_at); };

        return tieStruct(*this) == tieStruct(user);
    }

private:
    User() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
    bool active{false};
    bool email_verified{false};
    std::string created_at;
    std::string updated_at;
};
}
