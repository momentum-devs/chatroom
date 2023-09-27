#pragma once

#include <string>
#include <vector>

#include "../../entities/channel/Channel.h"
#include "payloads/CreateConversationPayload.h"
#include "payloads/DeleteConversationPayload.h"
#include "payloads/FindConversationByChannelIdPayload.h"
#include "payloads/FindConversationByIdPayload.h"
#include "payloads/FindConversationByUserAndRecipientIdsPayload.h"

namespace server::domain
{
class ConversationRepository
{
public:
    virtual ~ConversationRepository() = default;

    virtual Conversation createConversation(const CreateConversationPayload&) const = 0;
    virtual std::optional<domain::Conversation> findConversationById(const FindConversationByIdPayload&) const = 0;
    virtual std::optional<Conversation>
    findConversationByUserAndRecipientIds(const FindConversationByUserAndRecipientIdsPayload&) const = 0;
    virtual std::optional<Conversation>
    findConversationByChannelId(const FindConversationByChannelIdPayload&) const = 0;
    virtual void deleteConversation(const DeleteConversationPayload&) const = 0;
};
}
