#pragma once

#include <memory>

#include "DeleteMessageCommandHandler.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"

namespace server::application
{
class DeleteMessageCommandHandlerImpl : public DeleteMessageCommandHandler
{
public:
    explicit DeleteMessageCommandHandlerImpl(std::shared_ptr<domain::MessageRepository>);

    void execute(const DeleteMessageCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::MessageRepository> messageRepository;
};
}
