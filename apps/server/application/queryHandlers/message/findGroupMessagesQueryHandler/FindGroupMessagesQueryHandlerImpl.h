#pragma once

#include <memory>

#include "FindGroupMessagesQueryHandler.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"
#include "server/domain/repositories/userGroupRepository/UserGroupRepository.h"

namespace server::application
{
class FindGroupMessagesQueryHandlerImpl : public FindGroupMessagesQueryHandler
{
public:
    explicit FindGroupMessagesQueryHandlerImpl(std::shared_ptr<domain::MessageRepository>);

    FindGroupMessagesQueryHandlerResult execute(const FindGroupMessagesQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::MessageRepository> messageRepository;
};
}
