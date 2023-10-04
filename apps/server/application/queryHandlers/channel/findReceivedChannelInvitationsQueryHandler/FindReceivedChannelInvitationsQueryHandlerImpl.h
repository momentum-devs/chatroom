#pragma once

#include <memory>

#include "FindReceivedChannelInvitationsQueryHandler.h"
#include "server/domain/repositories/channelInvitationRepository/ChannelInvitationRepository.h"

namespace server::application
{
class FindReceivedChannelInvitationsQueryHandlerImpl : public FindReceivedChannelInvitationsQueryHandler
{
public:
    explicit FindReceivedChannelInvitationsQueryHandlerImpl(std::shared_ptr<domain::ChannelInvitationRepository>);

    FindReceivedChannelInvitationsQueryHandlerResult
    execute(const FindReceivedChannelInvitationsQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository;
};
}
