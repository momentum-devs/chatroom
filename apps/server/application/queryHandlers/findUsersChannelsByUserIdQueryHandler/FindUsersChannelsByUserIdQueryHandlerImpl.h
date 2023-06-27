#pragma once

#include <memory>

#include "FindUsersChannelsByUserIdQueryHandler.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class FindUsersChannelsByUserIdQueryHandlerImpl : public FindUsersChannelsByUserIdQueryHandler
{
public:
    explicit FindUsersChannelsByUserIdQueryHandlerImpl(std::shared_ptr<domain::UserChannelRepository>);

    FindUsersChannelsByUserIdQueryHandlerResult
    execute(const FindUsersChannelsByUserIdQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
