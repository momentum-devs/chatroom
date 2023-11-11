#pragma once

#include <string>
#include <vector>

#include "../../entities/message/Message.h"
#include "payloads/CreateMessagePayload.h"
#include "payloads/DeleteMessagePayload.h"
#include "payloads/FindMessageByIdPayload.h"
#include "payloads/FindMessagesByChannelIdPayload.h"
#include "payloads/FindMessagesByGroupIdPayload.h"
#include "payloads/UpdateMessagePayload.h"

namespace server::domain
{
class MessageRepository
{
public:
    virtual ~MessageRepository() = default;

    virtual std::shared_ptr<Message> createMessage(const CreateMessagePayload&) const = 0;
    virtual std::optional<std::shared_ptr<Message>> findMessageById(const FindMessageByIdPayload&) const = 0;
    virtual std::vector<std::shared_ptr<Message>>
    findMessagesByChannelId(const FindMessagesByChannelIdPayload&) const = 0;
    virtual std::vector<std::shared_ptr<Message>> findMessagesByGroupId(const FindMessagesByGroupIdPayload&) const = 0;
    virtual std::shared_ptr<Message> updateMessage(const UpdateMessagePayload&) const = 0;
    virtual void deleteMessage(const DeleteMessagePayload&) const = 0;
};
}
