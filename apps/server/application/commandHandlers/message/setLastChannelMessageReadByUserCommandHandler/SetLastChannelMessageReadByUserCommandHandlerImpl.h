#pragma once

#include <memory>

#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"
#include "SetLastChannelMessageReadByUserCommandHandler.h"

namespace server::application
{
class SetLastChannelMessageReadByUserCommandHandlerImpl : public SetLastChannelMessageReadByUserCommandHandler
{
public:
    SetLastChannelMessageReadByUserCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>,
                                                      std::shared_ptr<domain::UserRepository>,
                                                      std::shared_ptr<domain::UserChannelRepository>,
                                                      std::shared_ptr<domain::MessageRepository>);

    void execute(const SetLastChannelMessageReadByUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
    std::shared_ptr<domain::MessageRepository> messageRepository;
};
}
