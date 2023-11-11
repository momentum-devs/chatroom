#pragma once

#include "../../../../../domain/entities/message/Message.h"

namespace server::application
{
struct CreateChannelMessageCommandHandlerResult
{
    domain::Message message;
};
}
