#include "FindUsersChannelsByChannelIdQueryHandlerImpl.h"

namespace server::application
{
FindUsersChannelsByChannelIdQueryHandlerImpl::FindUsersChannelsByChannelIdQueryHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit)
    : userChannelRepository{std::move(userChannelRepositoryInit)}
{
}

FindUsersChannelsByChannelIdQueryHandlerResult FindUsersChannelsByChannelIdQueryHandlerImpl::execute(
    const FindUsersChannelsByChannelIdQueryHandlerPayload& payload) const
{
    const auto usersChannels = userChannelRepository->findUsersChannelsByChannelId({payload.channelId});

    return {usersChannels};
}
}
