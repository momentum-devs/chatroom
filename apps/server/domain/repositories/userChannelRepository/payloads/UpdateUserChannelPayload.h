#pragma once

#include <optional>
#include <string>

#include "../../../entities/userChannel/UserChannel.h"

namespace server::domain
{
struct UpdateUserChannelPayload
{
    UserChannel userChannel;
};
}
