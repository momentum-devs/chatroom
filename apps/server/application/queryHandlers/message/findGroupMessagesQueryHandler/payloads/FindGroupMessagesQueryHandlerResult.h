#pragma once

#include <vector>

#include "server/domain/entities/message/Message.h"

namespace server::application
{
struct FindGroupMessagesQueryHandlerResult
{
    std::vector<domain::Message> messages;
    unsigned totalCount;
};
}
