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
    User(std::string emailInit, std::string passwordInit, std::string nicknameInit)
        : email_{std::move(emailInit)}, password_{std::move(passwordInit)}, nickname_{std::move(nicknameInit)}
    {
    }

    std::string getEmail() const
    {
        return this->email_;
    }

    std::string getPassword() const
    {
        return this->password_;
    }

    std::string getNickname() const
    {
        return this->nickname_;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long id_;

    std::string email_;
    std::string password_;
    std::string nickname_;
};
}

#pragma clang diagnostic pop
