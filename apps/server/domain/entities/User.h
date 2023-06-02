#pragma once

#include <string>

namespace server::domain
{
class User
{
public:
    User(unsigned id, std::string email, std::string password, std::string nickname);

    unsigned getId() const;
private:
    unsigned id;
    std::string email;
    std::string password;
    std::string nickname;
};
}
