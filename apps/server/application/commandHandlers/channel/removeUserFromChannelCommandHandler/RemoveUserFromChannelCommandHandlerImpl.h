#pragma once

#include <memory>

#include "RemoveUserFromChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class RemoveUserFromChannelCommandHandlerImpl : public RemoveUserFromChannelCommandHandler
{
public:
    RemoveUserFromChannelCommandHandlerImpl(std::shared_ptr<domain::UserChannelRepository>,
                                            std::shared_ptr<domain::ChannelRepository>);

    void execute(const RemoveUserFromChannelCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
