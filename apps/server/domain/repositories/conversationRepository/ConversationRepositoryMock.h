#pragma once

#include <gmock/gmock.h>

#include "ConversationRepository.h"

namespace server::domain
{
class ConversationRepositoryMock : public ConversationRepository
{
public:
    MOCK_METHOD(Conversation, createConversation, (const CreateConversationPayload&), (const override));
    MOCK_METHOD(std::optional<Conversation>, findConversationById, (const FindConversationByIdPayload&),
                (const override));
    MOCK_METHOD(std::optional<Conversation>, findConversationByUserAndRecipientIds,
                (const FindConversationByUserAndRecipientIdsPayload&), (const override));
    MOCK_METHOD(std::optional<Conversation>, findConversationByChannelId, (const FindConversationByChannelIdPayload&),
                (const override));
    MOCK_METHOD(void, deleteConversation, (const DeleteConversationPayload&), (const override));
};
}
