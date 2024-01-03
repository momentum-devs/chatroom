#include "FindUsersBelongingToGroupQueryHandlerImpl.h"

#include "User.h"

namespace server::application
{
FindUsersBelongingToGroupQueryHandlerImpl::FindUsersBelongingToGroupQueryHandlerImpl(
    std::shared_ptr<domain::UserGroupRepository> userGroupRepositoryInit)
    : userGroupRepository{std::move(userGroupRepositoryInit)}
{
}

FindUsersBelongingToGroupQueryHandlerResult
FindUsersBelongingToGroupQueryHandlerImpl::execute(const FindUsersBelongingToGroupQueryHandlerPayload& payload) const
{
    const auto usersGroups = userGroupRepository->findUsersGroupsByGroupId({payload.groupId});

    std::vector<domain::User> users;

    for (const auto& userGroup : usersGroups)
    {
        users.push_back(*userGroup.getUser());
    }

    return {users};
}
}
