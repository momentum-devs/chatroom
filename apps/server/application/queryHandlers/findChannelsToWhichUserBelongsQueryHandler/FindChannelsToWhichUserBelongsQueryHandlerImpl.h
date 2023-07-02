#pragma once

#include <memory>

#include "FindChannelsToWhichUserBelongsQueryHandler.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class FindChannelsToWhichUserBelongsQueryHandlerImpl : public FindChannelsToWhichUserBelongsQueryHandler
{
public:
    explicit FindChannelsToWhichUserBelongsQueryHandlerImpl(std::shared_ptr<domain::UserChannelRepository>);

    FindChannelsToWhichUserBelongsQueryHandlerResult
    execute(const FindChannelsToWhichUserBelongsQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
