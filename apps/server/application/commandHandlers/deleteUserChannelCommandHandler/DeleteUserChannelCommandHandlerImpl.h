#pragma once

#include <memory>

#include "DeleteUserChannelCommandHandler.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class DeleteUserChannelCommandHandlerImpl : public DeleteUserChannelCommandHandler
{
public:
    explicit DeleteUserChannelCommandHandlerImpl(std::shared_ptr<domain::UserChannelRepository>);

    void execute(const DeleteUserChannelCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
