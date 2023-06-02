#pragma once

#include <string>

namespace server::domain
{
struct FindUserPayload
{
    const std::string email;
};
}
