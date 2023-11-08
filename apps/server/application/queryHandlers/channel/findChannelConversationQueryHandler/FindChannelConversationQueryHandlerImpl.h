#pragma once

#include <memory>

#include "FindChannelConversationQueryHandler.h"
#include "server/domain/repositories/conversationRepository/ConversationRepository.h"

namespace server::application
{
class FindChannelConversationQueryHandlerImpl : public FindChannelConversationQueryHandler
{
public:
    explicit FindChannelConversationQueryHandlerImpl(std::shared_ptr<domain::ConversationRepository>);

    FindChannelConversationQueryHandlerResult execute(const FindChannelConversationQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ConversationRepository> conversationRepository;
};
}
