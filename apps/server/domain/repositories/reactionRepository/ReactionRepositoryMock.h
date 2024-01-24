#pragma once

#include <gmock/gmock.h>

#include "ReactionRepository.h"

namespace server::domain
{
class ReactionRepositoryMock : public ReactionRepository
{
public:
    MOCK_METHOD(Reaction, createReaction, (const CreateReactionPayload&), (const override));
    MOCK_METHOD(std::vector<Reaction>, findReactions, (const FindReactionsPayload&), (const override));
    MOCK_METHOD(std::optional<Reaction>, findReactionById, (const FindReactionByIdPayload&), (const override));
    MOCK_METHOD(std::vector<Reaction>, findReactionsByMessageId, (const FindReactionsByMessageIdPayload&),
                (const override));
    MOCK_METHOD(Reaction, updateReaction, (const UpdateReactionPayload&), (const override));
    MOCK_METHOD(void, deleteReaction, (const DeleteReactionPayload&), (const override));
};
}
