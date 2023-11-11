#pragma once

#include <gmock/gmock.h>

#include "MessageRepository.h"

namespace server::domain
{
class MessageRepositoryMock : public MessageRepository
{
public:
    MOCK_METHOD(std::shared_ptr<Message>, createMessage, (const CreateMessagePayload&), (const override));
    MOCK_METHOD(std::optional<std::shared_ptr<Message>>, findMessageById, (const FindMessageByIdPayload&),
                (const override));
    MOCK_METHOD(std::vector<std::shared_ptr<Message>>, findMessagesByChannelId, (const FindMessagesByChannelIdPayload&),
                (const override));
    MOCK_METHOD(std::vector<std::shared_ptr<Message>>, findMessagesByGroupId, (const FindMessagesByGroupIdPayload&),
                (const override));
    MOCK_METHOD(std::shared_ptr<Message>, updateMessage, (const UpdateMessagePayload&), (const override));
    MOCK_METHOD(void, deleteMessage, (const DeleteMessagePayload&), (const override));
};
}
