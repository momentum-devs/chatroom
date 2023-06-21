#pragma once

#include <string>

namespace server::domain
{
struct FindUserByEmailPayload
{
    const std::string email;
};
}
