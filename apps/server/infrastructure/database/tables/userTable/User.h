#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

namespace server::infrastructure
{
#pragma db value(std::string) type("TEXT") id_type("VARCHAR(32)")

#pragma db object table("users")
class User
{
public:
    User(std::string idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit,
         std::string createdAtInit, std::string updatedAtInit)
        : id{std::move(idInit)},
          email{std::move(emailInit)},
          password{std::move(passwordInit)},
          nickname{std::move(nicknameInit)},
          created_at{createdAtInit},
          updated_at{updatedAtInit}
    {
    }

    std::string getId() const
    {
        return id;
    }

    std::string getEmail() const
    {
        return email;
    }

    std::string getPassword() const
    {
        return password;
    }

    std::string getNickname() const
    {
        return nickname;
    }

    std::string getCreatedAt() const
    {
        return created_at;
    }

    std::string getUpdatedAt() const
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

private:
    User() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
    std::string created_at;
    std::string updated_at;
};
}
