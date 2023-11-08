#pragma once

#include <vector>

#include "../../../../../domain/entities/conversation/Conversation.h"

namespace server::application
{
struct FindChannelConversationQueryHandlerResult
{
    domain::Conversation conversation;
};
}
