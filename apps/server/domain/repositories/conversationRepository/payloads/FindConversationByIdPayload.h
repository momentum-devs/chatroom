#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindConversationByIdPayload
{
    std::string id;
};
}
