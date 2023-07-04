#pragma once

#include <string>

namespace server::application
{
struct DeleteUserCommandHandlerPayload
{
    const std::string id;
};
}
