#pragma once

#include <string>
#include <vector>

#include "../../entities/channel/Channel.h"
#include "payloads/CreateUserChannelPayload.h"
#include "payloads/DeleteUserChannelPayload.h"
#include "payloads/FindUserChannelByIdPayload.h"
#include "payloads/FindUsersChannelsByChannelIdPayload.h"
#include "payloads/FindUsersChannelsByUserIdPayload.h"

namespace server::domain
{
class UserChannelRepository
{
public:
    virtual ~UserChannelRepository() = default;

    virtual UserChannel createUserChannel(const CreateUserChannelPayload&) const = 0;
    virtual std::optional<UserChannel> findUserChannelById(const FindUserChannelByIdPayload&) const = 0;
    virtual std::vector<UserChannel> findUsersChannelsByUserId(const FindUsersChannelsByUserIdPayload&) const = 0;
    virtual std::vector<UserChannel> findUsersChannelsByChannelId(const FindUsersChannelsByChannelIdPayload&) const = 0;
    virtual void deleteUserChannel(const DeleteUserChannelPayload&) const = 0;
};
}
