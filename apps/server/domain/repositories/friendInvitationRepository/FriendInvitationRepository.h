#pragma once

#include <string>
#include <vector>

#include "../../entities/friendInvitation/FriendInvitation.h"
#include "payloads/CreateFriendInvitationPayload.h"
#include "payloads/DeleteFriendInvitationPayload.h"
#include "payloads/FindFriendInvitationByIdPayload.h"
#include "payloads/FindFriendInvitationsByRecipientIdPayload.h"
#include "server/domain/repositories/friendInvitationRepository/payloads/FindFriendInvitationPayload.h"

namespace server::domain
{
class FriendInvitationRepository
{
public:
    virtual ~FriendInvitationRepository() = default;

    virtual FriendInvitation createFriendInvitation(const CreateFriendInvitationPayload&) const = 0;
    virtual std::optional<FriendInvitation> findFriendInvitationById(const FindFriendInvitationByIdPayload&) const = 0;
    virtual std::optional<FriendInvitation> findFriendInvitation(const FindFriendInvitationPayload&) const = 0;
    virtual std::vector<FriendInvitation>
    findFriendInvitationsByRecipientId(const FindFriendInvitationsByRecipientIdPayload&) const = 0;
    virtual void deleteFriendInvitation(const DeleteFriendInvitationPayload&) const = 0;
};
}
