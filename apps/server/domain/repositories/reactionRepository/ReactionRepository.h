#pragma once

#include <string>
#include <vector>

#include "../../entities/channel/Channel.h"
#include "payloads/CreateReactionPayload.h"
#include "payloads/DeleteReactionPayload.h"
#include "payloads/FindReactionByIdPayload.h"
#include "payloads/FindReactionsByMessageIdPayload.h"

namespace server::domain
{
class ReactionRepository
{
public:
    virtual ~ReactionRepository() = default;

    virtual Reaction createReaction(const CreateReactionPayload&) const = 0;
    virtual std::optional<Reaction> findReactionById(const FindReactionByIdPayload&) const = 0;
    virtual std::vector<Reaction> findReactionsByMessageId(const FindReactionsByMessageIdPayload&) const = 0;
    virtual void deleteReaction(const DeleteReactionPayload&) const = 0;
};
}
