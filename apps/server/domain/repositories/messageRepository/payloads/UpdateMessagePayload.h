#pragma once

#include <string>

#include "../../../entities/message/Message.h"

namespace server::domain
{
struct UpdateMessagePayload
{
    Message message;
};
}
