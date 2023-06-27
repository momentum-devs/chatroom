#pragma once

#include <string>

#include "../../../entities/userChannel/UserChannel.h"

namespace server::domain
{
struct DeleteUserChannelPayload
{
    UserChannel userChannel;
};
}
