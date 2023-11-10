#include "FindGroupsToWhichUserBelongsQueryHandlerImpl.h"

namespace server::application
{
FindGroupsToWhichUserBelongsQueryHandlerImpl::FindGroupsToWhichUserBelongsQueryHandlerImpl(
    std::shared_ptr<domain::UserGroupRepository> userGroupRepositoryInit)
    : userGroupRepository{std::move(userGroupRepositoryInit)}
{
}

FindGroupsToWhichUserBelongsQueryHandlerResult FindGroupsToWhichUserBelongsQueryHandlerImpl::execute(
    const FindGroupsToWhichUserBelongsQueryHandlerPayload& payload) const
{
    const auto usersGroups = userGroupRepository->findUsersGroupsByUserId({payload.userId});

    std::vector<domain::Group> groups;

    for (const auto& userGroup : usersGroups)
    {
        groups.push_back(*userGroup.getGroup());
    }

    return {groups};
}
}
