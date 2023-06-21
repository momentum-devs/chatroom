#pragma once

#include <memory>

#include "DeleteChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"

namespace server::application
{
class DeleteChannelCommandHandlerImpl : public DeleteChannelCommandHandler
{
public:
    explicit DeleteChannelCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>);

    void execute(const DeleteChannelCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
