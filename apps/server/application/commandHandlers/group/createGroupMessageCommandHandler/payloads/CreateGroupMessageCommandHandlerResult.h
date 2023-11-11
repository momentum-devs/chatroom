#pragma once

#include "../../../../../domain/entities/message/Message.h"

namespace server::application
{
struct CreateGroupMessageCommandHandlerResult
{
    domain::Message message;
};
}
