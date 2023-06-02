#pragma once

#include <string>

namespace server::domain
{
struct CreateUserPayload
{
    const std::string email;
    const std::string password;
    const std::string nickname;
};
}
