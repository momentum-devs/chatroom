#include "SetLastGroupMessageReadByUserCommandHandlerImpl.h"

#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
SetLastGroupMessageReadByUserCommandHandlerImpl::SetLastGroupMessageReadByUserCommandHandlerImpl(
    std::shared_ptr<domain::GroupRepository> groupRepositoryInit,
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::UserGroupRepository> userGroupRepositoryInit,
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit)
    : groupRepository{std::move(groupRepositoryInit)},
      userRepository{std::move(userRepositoryInit)},
      userGroupRepository{std::move(userGroupRepositoryInit)},
      messageRepository{std::move(messageRepositoryInit)}
{
}

void SetLastGroupMessageReadByUserCommandHandlerImpl::execute(
    const SetLastGroupMessageReadByUserCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format(
        "Setting last group message read by user... {{userId: {}, groupId: {}, messageId: {}}}", payload.userId,
        payload.groupId, payload.messageId);

    const auto message = messageRepository->findMessageById({payload.messageId});

    if (!message)
    {
        throw errors::ResourceNotFoundError{std::format("Message with id {} not found.", payload.messageId)};
    }

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.userId)};
    }

    const auto group = groupRepository->findGroupById({payload.groupId});

    if (!group)
    {
        throw errors::ResourceNotFoundError{std::format("Group with id {} not found.", payload.groupId)};
    }

    auto userGroup = userGroupRepository->findUserGroup({payload.userId, payload.groupId});

    if (!userGroup)
    {
        throw errors::ResourceNotFoundError{
            std::format("UserGroup not found. {{userId: {}, groupId: {}}}", payload.userId, payload.groupId)};
    }

    userGroup->setLastReadMessageId(payload.messageId);

    userGroupRepository->updateUserGroup({*userGroup});

    LOG_S(INFO) << std::format("Last read group message set. {{userId: {}, groupId: {}, messageId: {}}}",
                               payload.userId, payload.groupId, payload.messageId);
}

}
