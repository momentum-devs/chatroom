#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

namespace server::infrastructure
{
#pragma db value(std::string) type("TEXT") id_type("VARCHAR(128)")

#pragma db object table("users")
class User
{
public:
    User(std::string idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit)
        : id{std::move(idInit)},
          email{std::move(emailInit)},
          password{std::move(passwordInit)},
          nickname{std::move(nicknameInit)}
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
};
}

#pragma clang diagnostic pop
