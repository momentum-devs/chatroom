#pragma once

#include <string>

#include "../../../entities/conversation/Conversation.h"

namespace server::domain
{
struct DeleteConversationPayload
{
    Conversation conversation;
};
}
