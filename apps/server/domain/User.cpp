#include "User.h"

#include <utility>

namespace server::domain
{
User::User(std::string idInit, std::string emailInit, std::string passwordInit, std::string nicknameInit)
    : id{std::move(idInit)},
      email{std::move(emailInit)},
      password{std::move(passwordInit)},
      nickname{std::move(nicknameInit)}
{
}
}
