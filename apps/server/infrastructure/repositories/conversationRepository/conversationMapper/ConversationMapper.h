#pragma once

#include "../../../../domain/entities/conversation/Conversation.h"
#include "../../../database/entities/Conversation.h"

namespace server::infrastructure
{
class ConversationMapper
{
public:
    virtual ~ConversationMapper() = default;

    virtual domain::Conversation mapToDomainConversation(const Conversation&) const = 0;
};
}
