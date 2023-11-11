#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../../entities/channel/Channel.h"
#include "payloads/CreateChannelPayload.h"
#include "payloads/DeleteChannelPayload.h"
#include "payloads/FindChannelByIdPayload.h"
#include "payloads/UpdateChannelPayload.h"

namespace server::domain
{
class ChannelRepository
{
public:
    virtual ~ChannelRepository() = default;

    virtual std::shared_ptr<Channel> createChannel(const CreateChannelPayload&) const = 0;
    virtual std::optional<std::shared_ptr<Channel>> findChannelById(const FindChannelByIdPayload&) const = 0;
    virtual std::shared_ptr<Channel> updateChannel(const UpdateChannelPayload&) const = 0;
    virtual void deleteChannel(const DeleteChannelPayload&) const = 0;
};
}
