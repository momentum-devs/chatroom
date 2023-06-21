#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../../entities/channel/Channel.h"
#include "payloads/CreateChannelPayload.h"
#include "payloads/DeleteChannelPayload.h"
#include "payloads/FindChannelByIdPayload.h"

namespace server::domain
{
class ChannelRepository
{
public:
    virtual ~ChannelRepository() = default;

    virtual Channel createChannel(const CreateChannelPayload&) const = 0;
    virtual std::optional<Channel> findChannelById(const FindChannelByIdPayload&) const = 0;
    virtual void deleteChannel(const DeleteChannelPayload&) const = 0;
};
}
