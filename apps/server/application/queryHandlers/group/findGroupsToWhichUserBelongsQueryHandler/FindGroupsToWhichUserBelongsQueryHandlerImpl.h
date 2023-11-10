#pragma once

#include <memory>

#include "FindGroupsToWhichUserBelongsQueryHandler.h"
#include "server/domain/repositories/userGroupRepository/UserGroupRepository.h"

namespace server::application
{
class FindGroupsToWhichUserBelongsQueryHandlerImpl : public FindGroupsToWhichUserBelongsQueryHandler
{
public:
    explicit FindGroupsToWhichUserBelongsQueryHandlerImpl(std::shared_ptr<domain::UserGroupRepository>);

    FindGroupsToWhichUserBelongsQueryHandlerResult
    execute(const FindGroupsToWhichUserBelongsQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserGroupRepository> userGroupRepository;
};
}
