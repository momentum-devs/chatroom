#pragma once

#include <memory>

#include "FindUsersChannelsByChannelIdQueryHandler.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class FindUsersChannelsByChannelIdQueryHandlerImpl : public FindUsersChannelsByChannelIdQueryHandler
{
public:
    explicit FindUsersChannelsByChannelIdQueryHandlerImpl(std::shared_ptr<domain::UserChannelRepository>);

    FindUsersChannelsByChannelIdQueryHandlerResult
    execute(const FindUsersChannelsByChannelIdQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
