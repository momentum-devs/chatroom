#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "../../../domain/repositories/channelRepository/ChannelRepository.h"
#include "channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ChannelRepositoryImpl : public domain::ChannelRepository
{
public:
    ChannelRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::shared_ptr<ChannelMapper>,
                          std::shared_ptr<UserMapper>);

    std::shared_ptr<domain::Channel> createChannel(const domain::CreateChannelPayload&) const override;
    std::optional<std::shared_ptr<domain::Channel>>
    findChannelById(const domain::FindChannelByIdPayload&) const override;
    void deleteChannel(const domain::DeleteChannelPayload&) const override;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<ChannelMapper> channelMapper;
    std::shared_ptr<UserMapper> userMapper;
};
}
