#pragma once

#include <string>
#include <vector>

#include "../../entities/channelInvitation/ChannelInvitation.h"
#include "payloads/CreateChannelInvitationPayload.h"
#include "payloads/DeleteChannelInvitationPayload.h"
#include "payloads/FindChannelInvitationByIdPayload.h"
#include "payloads/FindChannelInvitationsByRecipientIdPayload.h"
#include "server/domain/repositories/channelInvitationRepository/payloads/FindChannelInvitationPayload.h"

namespace server::domain
{
class ChannelInvitationRepository
{
public:
    virtual ~ChannelInvitationRepository() = default;

    virtual ChannelInvitation createChannelInvitation(const CreateChannelInvitationPayload&) const = 0;
    virtual std::optional<ChannelInvitation> findChannelInvitation(const FindChannelInvitationPayload&) const = 0;
    virtual std::optional<ChannelInvitation>
    findChannelInvitationById(const FindChannelInvitationByIdPayload&) const = 0;
    virtual std::vector<ChannelInvitation>
    findChannelInvitationsByRecipientId(const FindChannelInvitationsByRecipientIdPayload&) const = 0;
    virtual void deleteChannelInvitation(const DeleteChannelInvitationPayload&) const = 0;
};
}
