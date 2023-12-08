#pragma once

#include <gmock/gmock.h>

#include "MessageMapper.h"

namespace server::infrastructure
{
class MessageMapperMock : public MessageMapper
{
public:
    MOCK_METHOD(std::shared_ptr<domain::Message>, mapToDomainMessage, (std::shared_ptr<Message>), (const override));
    MOCK_METHOD(std::shared_ptr<Message>, mapToPersistenceMessage, (std::shared_ptr<domain::Message>),
                (const override));
};
}
