#pragma once

#include <gmock/gmock.h>

#include "FriendInvitationRepository.h"

namespace server::domain
{
class FriendInvitationRepositoryMock : public FriendInvitationRepository
{
public:
    MOCK_METHOD(FriendInvitation, createFriendInvitation, (const CreateFriendInvitationPayload&), (const override));
    MOCK_METHOD(std::optional<FriendInvitation>, findFriendInvitationById, (const FindFriendInvitationByIdPayload&),
                (const override));
    MOCK_METHOD(std::vector<FriendInvitation>, findFriendInvitationsByRecipientId,
                (const FindFriendInvitationsByRecipientIdPayload&), (const override));
    MOCK_METHOD(void, deleteFriendInvitation, (const DeleteFriendInvitationPayload&), (const override));
};
}
