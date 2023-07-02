#include "FindUsersBelongingToChannelQueryHandlerImpl.h"

#include "User.h"

namespace server::application
{
FindUsersBelongingToChannelQueryHandlerImpl::FindUsersBelongingToChannelQueryHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit)
    : userChannelRepository{std::move(userChannelRepositoryInit)}
{
}

FindUsersBelongingToChannelQueryHandlerResult FindUsersBelongingToChannelQueryHandlerImpl::execute(
    const FindUsersBelongingToChannelQueryHandlerPayload& payload) const
{
    const auto usersChannels = userChannelRepository->findUsersChannelsByChannelId({payload.channelId});

    std::vector<domain::User> users;

    for (const auto& userChannel : usersChannels)
    {
        users.push_back(*userChannel.getUser());
    }

    return {users};
}
}
