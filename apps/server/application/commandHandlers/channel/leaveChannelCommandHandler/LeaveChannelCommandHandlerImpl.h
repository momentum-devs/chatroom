#pragma once

#include <memory>

#include "LeaveChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class LeaveChannelCommandHandlerImpl : public LeaveChannelCommandHandler
{
public:
    LeaveChannelCommandHandlerImpl(std::shared_ptr<domain::UserChannelRepository>,
                                   std::shared_ptr<domain::ChannelRepository>);

    void execute(const LeaveChannelCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
