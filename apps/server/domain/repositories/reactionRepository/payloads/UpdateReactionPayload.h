#pragma once

#include <string>

#include "../../../entities/reaction/Reaction.h"

namespace server::domain
{
struct UpdateReactionPayload
{
    Reaction reaction;
};
}
