#pragma once

#include <string>

namespace server::domain
{
struct FindReactionsByMessageIdPayload
{
    std::string messageId;
};
}
