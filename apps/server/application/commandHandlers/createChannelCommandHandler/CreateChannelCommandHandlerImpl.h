#pragma once

#include <memory>

#include "CreateChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"

namespace server::application
{
class CreateChannelCommandHandlerImpl : public CreateChannelCommandHandler
{
public:
    explicit CreateChannelCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>);

    CreateChannelCommandHandlerResult execute(const CreateChannelCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
