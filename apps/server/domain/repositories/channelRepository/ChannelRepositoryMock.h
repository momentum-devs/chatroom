#pragma once

#include <gmock/gmock.h>

#include "ChannelRepository.h"

namespace server::domain
{
class ChannelRepositoryMock : public ChannelRepository
{
public:
    MOCK_METHOD(std::shared_ptr<Channel>, createChannel, (const CreateChannelPayload&), (const override));
    MOCK_METHOD(std::optional<std::shared_ptr<Channel>>, findChannelById, (const FindChannelByIdPayload&),
                (const override));
    MOCK_METHOD(std::shared_ptr<Channel>, updateChannel, (const UpdateChannelPayload&), (const override));
    MOCK_METHOD(void, deleteChannel, (const DeleteChannelPayload&), (const override));
};
}
