#pragma once

#include <memory>

#include "FindChannelByIdQueryHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"

namespace server::application
{
class FindChannelByIdQueryHandlerImpl : public FindChannelByIdQueryHandler
{
public:
    explicit FindChannelByIdQueryHandlerImpl(std::shared_ptr<domain::ChannelRepository>);

    FindChannelByIdQueryHandlerResult execute(const FindChannelByIdQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
