#pragma once

#include <memory>

#include "FindChannelMessagesQueryHandler.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"

namespace server::application
{
class FindChannelMessagesQueryHandlerImpl : public FindChannelMessagesQueryHandler
{
public:
    explicit FindChannelMessagesQueryHandlerImpl(std::shared_ptr<domain::MessageRepository>);

    FindChannelMessagesQueryHandlerResult execute(const FindChannelMessagesQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::MessageRepository> messageRepository;
};
}
