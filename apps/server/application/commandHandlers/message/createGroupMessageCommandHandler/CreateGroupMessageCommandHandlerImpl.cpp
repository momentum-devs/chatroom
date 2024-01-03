#include "CreateGroupMessageCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateGroupMessageCommandHandlerImpl::CreateGroupMessageCommandHandlerImpl(
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

CreateGroupMessageCommandHandlerResult
CreateGroupMessageCommandHandlerImpl::execute(const CreateGroupMessageCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format(R"(Creating group message "{}" by sender with id "{}" to group with id "{}"...)",
                               payload.content, payload.senderId, payload.groupId);

    const auto sender = userRepository->findUserById({payload.senderId});

    if (!sender)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.senderId)};
    }

    const auto group = groupRepository->findGroupById({payload.groupId});

    if (!group)
    {
        throw errors::ResourceNotFoundError{fmt::format("Group with id {} not found.", payload.groupId)};
    }

    const auto userGroups = userGroupRepository->findUsersGroupsByUserId({payload.senderId});

    const auto userIsMemberOfGroup = std::any_of(userGroups.begin(), userGroups.end(),
                                                 [&payload](const domain::UserGroup& userGroup) {
                                                     return userGroup.getUser()->getId() == payload.senderId &&
                                                            userGroup.getGroup()->getId() == payload.groupId;
                                                 });

    if (!userIsMemberOfGroup)
    {
        throw errors::OperationNotValidError{
            fmt::format("Sender {} is not a member of group {}.", sender->get()->getEmail(), group->get()->getId())};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto messageId = uuid.str();

    const auto message = messageRepository->createMessage({messageId, payload.content, *sender, std::nullopt, group});

    LOG_S(INFO) << fmt::format(R"(Message from sender with id "{}" with content "{}" created.)", payload.senderId,
                               payload.content);

    return {*message};
}

}
