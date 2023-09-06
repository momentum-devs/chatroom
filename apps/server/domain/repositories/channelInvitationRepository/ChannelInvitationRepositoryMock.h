#pragma once

#include <gmock/gmock.h>

#include "ChannelInvitationRepository.h"

namespace server::domain
{
class ChannelInvitationRepositoryMock : public ChannelInvitationRepository
{
public:
    MOCK_METHOD(ChannelInvitation, createChannelInvitation, (const CreateChannelInvitationPayload&), (const override));
    MOCK_METHOD(std::optional<ChannelInvitation>, findChannelInvitationById, (const FindChannelInvitationByIdPayload&),
                (const override));
    MOCK_METHOD(std::vector<ChannelInvitation>, findChannelInvitationsByRecipientId,
                (const FindChannelInvitationsByRecipientIdPayload&), (const override));
    MOCK_METHOD(void, deleteChannelInvitation, (const DeleteChannelInvitationPayload&), (const override));
};
}
