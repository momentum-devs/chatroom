#pragma once

#include <gmock/gmock.h>

#include "ChannelMapper.h"

namespace server::infrastructure
{
class ChannelMapperMock : public ChannelMapper
{
public:
    MOCK_METHOD(std::shared_ptr<domain::Channel>, mapToDomainChannel, (const std::shared_ptr<Channel>&),
                (const override));
    MOCK_METHOD(std::shared_ptr<Channel>, mapToPersistenceChannel, (const std::shared_ptr<domain::Channel>&),
                (const override));
};
}
