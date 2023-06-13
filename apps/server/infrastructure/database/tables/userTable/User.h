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

#pragma db object
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
        return this->id;
    }

    std::string getEmail() const
    {
        return this->email;
    }

    std::string getPassword() const
    {
        return this->password;
    }

    std::string getNickname() const
    {
        return this->nickname;
    }

private:
    friend class odb::access;

#pragma db id
    std::string id;
    
    std::string email;
    std::string password;
    std::string nickname;
};
}

#pragma clang diagnostic pop
