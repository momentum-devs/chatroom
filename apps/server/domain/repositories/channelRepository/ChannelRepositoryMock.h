#pragma once

#include <gmock/gmock.h>

#include "ChannelRepository.h"

namespace server::domain
{
class ChannelRepositoryMock : public ChannelRepository
{
public:
    MOCK_METHOD(Channel, createChannel, (const CreateChannelPayload&), (const override));
    MOCK_METHOD(std::optional<Channel>, findChannelById, (const FindChannelByIdPayload&), (const override));
    MOCK_METHOD(void, deleteChannel, (const DeleteChannelPayload&), (const override));
};
}
