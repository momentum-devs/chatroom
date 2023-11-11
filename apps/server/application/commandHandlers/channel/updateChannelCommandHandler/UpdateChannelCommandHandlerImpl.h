#pragma once

#include <memory>

#include "server/application/services/hashService/HashService.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "UpdateChannelCommandHandler.h"

namespace server::application
{
class UpdateChannelCommandHandlerImpl : public UpdateChannelCommandHandler
{
public:
    explicit UpdateChannelCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>);

    UpdateChannelCommandHandlerResult execute(const UpdateChannelCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
