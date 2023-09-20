#pragma once

#include <memory>

#include "RemoveUserFromChannelCommandHandler.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class RemoveUserFromChannelCommandHandlerImpl : public RemoveUserFromChannelCommandHandler
{
public:
    explicit RemoveUserFromChannelCommandHandlerImpl(std::shared_ptr<domain::UserChannelRepository>);

    void execute(const RemoveUserFromChannelCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
