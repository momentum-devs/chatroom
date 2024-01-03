#include "DeleteUserCommandHandlerImpl.h"

#include "fmt/format.h"
#include <unordered_map>

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteUserCommandHandlerImpl::DeleteUserCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::UserGroupRepository> userGroupRepositoryInit,
    std::shared_ptr<domain::GroupRepository> groupRepositoryInit)
    : userRepository{std::move(userRepositoryInit)},
      userGroupRepository{std::move(userGroupRepositoryInit)},
      groupRepository{std::move(groupRepositoryInit)}
{
}

void DeleteUserCommandHandlerImpl::execute(const DeleteUserCommandHandlerPayload& payload)
{
    LOG_S(INFO) << fmt::format("Deleting user with id \"{}\"...", payload.id);

    const auto existingUser = userRepository->findUserById({payload.id});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.id)};
    }

    LOG_S(INFO) << fmt::format("Deleting user groups... {{userId: {}}}", payload.id);

    const auto usersGroups = userGroupRepository->findUsersGroupsByUserId({payload.id});

    std::unordered_map<std::string, int> groupIdToNumberOfMembers;

    for (const auto& userGroup : usersGroups)
    {
        groupIdToNumberOfMembers[userGroup.getGroup()->getId()]++;
    }

    std::vector<std::string> groupsIdsToWhichUserBelongs;

    for (const auto& userGroup : usersGroups)
    {
        if (userGroup.getUser()->getId() == payload.id)
        {
            groupsIdsToWhichUserBelongs.push_back(userGroup.getGroup()->getId());
        }
    }

    if (groupsIdsToWhichUserBelongs.empty())
    {
        LOG_S(INFO) << fmt::format("No groups to delete. {{userId: {}}}", payload.id);

        return;
    }

    groupRepository->deleteGroups({groupsIdsToWhichUserBelongs});

    LOG_S(INFO) << fmt::format("User groups deleted. {{userId: {}, numberOfGroups: {}}}", payload.id,
                               groupsIdsToWhichUserBelongs.size());

    userRepository->deleteUser({**existingUser});

    LOG_S(INFO) << fmt::format("User with id \"{}\" deleted.", payload.id);
}
}
