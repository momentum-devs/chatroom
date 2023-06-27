#pragma once

#include "../../../../domain/entities/userChannel/UserChannel.h"

namespace server::application
{
struct CreateUserChannelCommandHandlerResult
{
    domain::UserChannel userChannel;
};
}
