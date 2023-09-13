#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "server/domain/repositories/friendInvitationRepository/FriendInvitationRepository.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class FriendInvitationRepositoryImpl : public domain::FriendInvitationRepository
{
public:
    FriendInvitationRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::shared_ptr<FriendInvitationMapper>,
                                   std::shared_ptr<UserMapper>);

    domain::FriendInvitation createFriendInvitation(const domain::CreateFriendInvitationPayload&) const;
    std::optional<domain::FriendInvitation>
    findFriendInvitationById(const domain::FindFriendInvitationByIdPayload&) const;
    std::vector<domain::FriendInvitation>
    findFriendInvitationsByRecipientId(const domain::FindFriendInvitationsByRecipientIdPayload&) const;
    void deleteFriendInvitation(const domain::DeleteFriendInvitationPayload&) const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<FriendInvitationMapper> friendInvitationMapper;
    std::shared_ptr<UserMapper> userMapper;
};
}
