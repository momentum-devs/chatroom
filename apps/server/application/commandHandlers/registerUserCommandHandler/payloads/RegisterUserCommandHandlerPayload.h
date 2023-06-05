#pragma once

#include <string>

namespace server::application
{
struct RegisterUserCommandHandlerPayload
{
    const std::string email;
    const std::string password;
};
}
