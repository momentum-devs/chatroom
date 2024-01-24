#pragma once

#include <string>

namespace server::domain
{
struct FindReactionPayload
{
    std::string userId;
    std::string messageId;
};
}
