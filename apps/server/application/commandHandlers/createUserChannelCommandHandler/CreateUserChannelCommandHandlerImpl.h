#pragma once

#include <memory>

#include "CreateUserChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateUserChannelCommandHandlerImpl : public CreateUserChannelCommandHandler
{
public:
    CreateUserChannelCommandHandlerImpl(std::shared_ptr<domain::UserChannelRepository>,
                                        std::shared_ptr<domain::UserRepository>,
                                        std::shared_ptr<domain::ChannelRepository>);

    CreateUserChannelCommandHandlerResult execute(const CreateUserChannelCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
