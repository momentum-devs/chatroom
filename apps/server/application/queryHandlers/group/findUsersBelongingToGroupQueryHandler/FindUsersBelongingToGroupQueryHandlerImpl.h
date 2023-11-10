#pragma once

#include <memory>

#include "FindUsersBelongingToGroupQueryHandler.h"
#include "server/domain/repositories/userGroupRepository/UserGroupRepository.h"

namespace server::application
{
class FindUsersBelongingToGroupQueryHandlerImpl : public FindUsersBelongingToGroupQueryHandler
{
public:
    explicit FindUsersBelongingToGroupQueryHandlerImpl(std::shared_ptr<domain::UserGroupRepository>);

    FindUsersBelongingToGroupQueryHandlerResult
    execute(const FindUsersBelongingToGroupQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserGroupRepository> userGroupRepository;
};
}
