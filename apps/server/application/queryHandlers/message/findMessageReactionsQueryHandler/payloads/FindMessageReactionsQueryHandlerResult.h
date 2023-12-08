#pragma once

#include <vector>

#include "server/domain/entities/reaction/Reaction.h"

namespace server::application
{
struct FindMessageReactionsQueryHandlerResult
{
    std::vector<domain::Reaction> reactions;
};
}
