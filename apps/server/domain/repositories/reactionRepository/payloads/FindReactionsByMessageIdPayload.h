#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindReactionsByMessageIdPayload
{
    std::string messageId;
};
}
