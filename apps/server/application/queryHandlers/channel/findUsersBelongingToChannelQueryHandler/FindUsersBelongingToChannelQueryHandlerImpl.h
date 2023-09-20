#pragma once

#include <memory>

#include "FindUsersBelongingToChannelQueryHandler.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class FindUsersBelongingToChannelQueryHandlerImpl : public FindUsersBelongingToChannelQueryHandler
{
public:
    explicit FindUsersBelongingToChannelQueryHandlerImpl(std::shared_ptr<domain::UserChannelRepository>);

    FindUsersBelongingToChannelQueryHandlerResult
    execute(const FindUsersBelongingToChannelQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
