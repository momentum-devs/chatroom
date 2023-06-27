#include "FindUsersChannelsByUserIdQueryHandlerImpl.h"

namespace server::application
{
FindUsersChannelsByUserIdQueryHandlerImpl::FindUsersChannelsByUserIdQueryHandlerImpl(
    std::shared_ptr<domain::UserChannelRepository> userChannelRepositoryInit)
    : userChannelRepository{std::move(userChannelRepositoryInit)}
{
}

FindUsersChannelsByUserIdQueryHandlerResult
FindUsersChannelsByUserIdQueryHandlerImpl::execute(const FindUsersChannelsByUserIdQueryHandlerPayload& payload) const
{
    const auto usersChannels = userChannelRepository->findUsersChannelsByUserId({payload.userId});

    return {usersChannels};
}
}
