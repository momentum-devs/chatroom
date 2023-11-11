#pragma once

#include <memory>

#include "server/application/services/hashService/HashService.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"
#include "UpdateMessageCommandHandler.h"

namespace server::application
{
class UpdateMessageCommandHandlerImpl : public UpdateMessageCommandHandler
{
public:
    explicit UpdateMessageCommandHandlerImpl(std::shared_ptr<domain::MessageRepository>);

    UpdateMessageCommandHandlerResult execute(const UpdateMessageCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::MessageRepository> messageRepository;
};
}
