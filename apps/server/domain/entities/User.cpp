#include "User.h"

namespace server::domain
{
User::User(unsigned idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit)
    : id{idInit},
      email{std::move(emailInit)},
      password{std::move(passwordInit)},
      nickname{std::move(nicknameInit)}
{
}

unsigned User::getId() const
{
    return id;
}
}
