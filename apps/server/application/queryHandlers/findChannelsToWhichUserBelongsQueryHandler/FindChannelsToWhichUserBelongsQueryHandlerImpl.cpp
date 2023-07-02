#include "FindChannelsToWhichUserBelongsQueryHandlerImpl.h"

namespace server::application
{
FindChannelsToWhichUserBelongsQueryHandlerImpl::FindChannelsToWhichUserBelongsQueryHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit)
    : userChannelRepository{std::move(userChannelRepositoryInit)}
{
}

FindChannelsToWhichUserBelongsQueryHandlerResult FindChannelsToWhichUserBelongsQueryHandlerImpl::execute(
    const FindChannelsToWhichUserBelongsQueryHandlerPayload& payload) const
{
    const auto usersChannels = userChannelRepository->findUsersChannelsByUserId({payload.userId});

    std::vector<domain::Channel> channels;

    for (const auto& userChannel : usersChannels)
    {
        channels.push_back(*userChannel.getChannel());
    }

    return {channels};
}
}
