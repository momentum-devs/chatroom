#pragma once

#include "../../../../../domain/entities/reaction/Reaction.h"

namespace server::application
{
struct CreateMessageReactionCommandHandlerResult
{
    domain::Reaction reaction;
};
}
