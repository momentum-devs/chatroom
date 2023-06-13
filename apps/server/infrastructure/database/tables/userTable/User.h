#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#include <memory>
#include <odb/core.hxx>
#include <string>

namespace server::infrastructure
{
#pragma db object
class User
{
public:
    User(std::string emailInit, std::string passwordInit, std::string nicknameInit)
        : email{std::move(emailInit)}, password{std::move(passwordInit)}, nickname{std::move(nicknameInit)}
    {
    }

    std::string getNickname() const
    {
        return this->nickname;
    }

    std::string getEmail() const
    {
        return this->email;
    }

private:
    friend class odb::access;

#pragma db id
    std::string email;
    std::string password;
    std::string nickname;
};
}

#pragma clang diagnostic pop
