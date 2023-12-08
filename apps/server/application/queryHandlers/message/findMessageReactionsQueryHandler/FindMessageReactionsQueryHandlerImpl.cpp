#include "FindMessageReactionsQueryHandlerImpl.h"

#include "User.h"

namespace server::application
{
FindMessageReactionsQueryHandlerImpl::FindMessageReactionsQueryHandlerImpl(
    std::shared_ptr<domain::ReactionRepository> reactionRepositoryInit)
    : reactionRepository{std::move(reactionRepositoryInit)}
{
}

FindMessageReactionsQueryHandlerResult
FindMessageReactionsQueryHandlerImpl::execute(const FindMessageReactionsQueryHandlerPayload& payload) const
{
    const auto messageReactions = reactionRepository->findReactionsByMessageId({payload.messageId});

    std::vector<domain::Reaction> result;

    result.reserve(messageReactions.size());

    for (const auto& messageReaction : messageReactions)
    {
        result.push_back(messageReaction);
    }

    return {result};
}
}
