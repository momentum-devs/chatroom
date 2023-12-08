#pragma once

#include "../../../../domain/entities/message/Message.h"
#include "../../../database/entities/Message.h"

namespace server::infrastructure
{
class MessageMapper
{
public:
    virtual ~MessageMapper() = default;

    virtual std::shared_ptr<domain::Message> mapToDomainMessage(std::shared_ptr<Message>) const = 0;
    virtual std::shared_ptr<Message> mapToPersistenceMessage(const std::shared_ptr<domain::Message>) const = 0;
};
}
