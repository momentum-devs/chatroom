#pragma once

#include <string>

namespace server::domain
{
struct FindReactionsPayload
{
    std::string userId;
    std::string messageId;
};
}
