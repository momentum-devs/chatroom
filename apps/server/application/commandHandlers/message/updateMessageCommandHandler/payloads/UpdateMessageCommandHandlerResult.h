#pragma once

#include "../../../../../domain/entities/message/Message.h"

namespace server::application
{
struct UpdateMessageCommandHandlerResult
{
    domain::Message message;
};
}
