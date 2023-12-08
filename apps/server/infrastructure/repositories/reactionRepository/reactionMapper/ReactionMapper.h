#pragma once

#include "../../../../domain/entities/reaction/Reaction.h"
#include "../../../database/entities/Reaction.h"

namespace server::infrastructure
{
class ReactionMapper
{
public:
    virtual ~ReactionMapper() = default;

    virtual domain::Reaction mapToDomainReaction(const Reaction&) const = 0;
};
}
