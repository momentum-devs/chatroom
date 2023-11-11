#pragma once

#include <memory>

#include "CreateChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateChannelCommandHandlerImpl : public CreateChannelCommandHandler
{
public:
    CreateChannelCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>, std::shared_ptr<domain::UserRepository>,
                                    std::shared_ptr<domain::UserChannelRepository>);

    CreateChannelCommandHandlerResult execute(const CreateChannelCommandHandlerPayload&) const override;

private:
    void addUserToChannel(const std::string& userId, const std::string& channelId) const;

    std::shared_ptr<domain::ChannelRepository> channelRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
