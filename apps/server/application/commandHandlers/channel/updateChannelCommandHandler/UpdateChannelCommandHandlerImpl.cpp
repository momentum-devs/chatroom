#include "UpdateChannelCommandHandlerImpl.h"

#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
UpdateChannelCommandHandlerImpl::UpdateChannelCommandHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit)
    : channelRepository{std::move(channelRepositoryInit)}
{
}

UpdateChannelCommandHandlerResult
UpdateChannelCommandHandlerImpl::execute(const UpdateChannelCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format(R"(Updating channel with id "{}" with name "{}"...)", payload.id, payload.name);

    auto existingChannel = channelRepository->findChannelById({payload.id});

    if (!existingChannel)
    {
        throw errors::ResourceNotFoundError{std::format("Channel with id \"{}\" not found.", payload.id)};
    }

    const auto creatorId = existingChannel->get()->getCreator()->getId();

    if (payload.requesterUserId != creatorId)
    {
        throw errors::OperationNotValidError{std::format("User with id {} is not creator of the channel with id {}.",
                                                         payload.requesterUserId, payload.id)};
    }

    existingChannel->get()->setName(payload.name);

    const auto channel = channelRepository->updateChannel({**existingChannel});

    LOG_S(INFO) << std::format("Channel with id \"{}\" updated.", payload.id);

    return {*channel};
}
}
