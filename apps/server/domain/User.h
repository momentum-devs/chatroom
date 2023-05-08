#pragma once

#include <string>

namespace server::domain
{
class User
{
public:
    User(std::string id, std::string email, std::string password, std::string nickname);

private:
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
};
}
