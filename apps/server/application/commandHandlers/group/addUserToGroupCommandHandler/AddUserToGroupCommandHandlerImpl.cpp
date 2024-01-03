#include "AddUserToGroupCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "fmt/format.h"

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
AddUserToGroupCommandHandlerImpl::AddUserToGroupCommandHandlerImpl(
    std::shared_ptr<domain::UserGroupRepository> userGroupRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::GroupRepository> groupRepositoryInit)
    : userGroupRepository{std::move(userGroupRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      groupRepository{std::move(groupRepositoryInit)}
{
}

void AddUserToGroupCommandHandlerImpl::execute(const AddUserToGroupCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Adding user to group... {{userId: {}, groupId: {}}}", payload.userId, payload.groupId);

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.userId)};
    }

    const auto group = groupRepository->findGroupById({payload.groupId});

    if (!group)
    {
        throw errors::ResourceNotFoundError{fmt::format("Group with id {} not found.", payload.groupId)};
    }

    const auto existingUserGroup = userGroupRepository->findUserGroup({payload.userId, payload.groupId});

    if (existingUserGroup)
    {
        throw errors::OperationNotValidError{
            fmt::format("User with id {} is already in group with id {}.", payload.userId, payload.groupId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto userGroupId = uuid.str();

    const auto userGroup = userGroupRepository->createUserGroup({userGroupId, *user, *group});

    LOG_S(INFO) << fmt::format("User added to group. {{userId: {}, groupId: {}}}", userGroup.getUser()->getId(),
                               userGroup.getGroup()->getId());
}
}
