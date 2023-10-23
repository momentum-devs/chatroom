#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "server/domain/repositories/channelInvitationRepository/ChannelInvitationRepository.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ChannelInvitationRepositoryImpl : public domain::ChannelInvitationRepository
{
public:
    ChannelInvitationRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::shared_ptr<ChannelInvitationMapper>,
                                    std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>);

    domain::ChannelInvitation createChannelInvitation(const domain::CreateChannelInvitationPayload&) const;
    std::optional<domain::ChannelInvitation> findChannelInvitation(const domain::FindChannelInvitationPayload&) const;
    std::optional<domain::ChannelInvitation>
    findChannelInvitationById(const domain::FindChannelInvitationByIdPayload&) const;
    std::vector<domain::ChannelInvitation>
    findChannelInvitationsByRecipientId(const domain::FindChannelInvitationsByRecipientIdPayload&) const;
    void deleteChannelInvitation(const domain::DeleteChannelInvitationPayload&) const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<ChannelInvitationMapper> channelInvitationMapper;
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
};
}
