#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "../../../domain/repositories/channelRepository/ChannelRepository.h"
#include "channelMapper/ChannelMapper.h"

namespace server::infrastructure
{
class ChannelRepositoryImpl : public domain::ChannelRepository
{
public:
    ChannelRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::unique_ptr<ChannelMapper>);

    domain::Channel createChannel(const domain::CreateChannelPayload&) const override;
    std::optional<domain::Channel> findChannelById(const domain::FindChannelByIdPayload&) const override;
    void deleteChannel(const domain::DeleteChannelPayload&) const override;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::unique_ptr<ChannelMapper> channelMapper;
};
}
