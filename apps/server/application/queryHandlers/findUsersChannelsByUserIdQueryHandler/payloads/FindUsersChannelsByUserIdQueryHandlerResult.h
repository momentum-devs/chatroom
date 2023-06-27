#pragma once

#include <vector>

#include "../../../../domain/entities/userChannel/UserChannel.h"

namespace server::application
{
struct FindUsersChannelsByUserIdQueryHandlerResult
{
    std::vector<domain::UserChannel> usersChannels;
};
}
