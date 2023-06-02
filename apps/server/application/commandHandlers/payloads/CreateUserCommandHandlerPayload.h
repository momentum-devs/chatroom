#pragma once

#include <string>

namespace server::application
{
struct CreateUserCommandHandlerPayload
{
    const std::string email;
    const std::string password;
};
}
