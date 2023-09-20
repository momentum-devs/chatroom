#pragma once

#include <memory>

#include "AddUserToChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class AddUserToChannelCommandHandlerImpl : public AddUserToChannelCommandHandler
{
public:
    AddUserToChannelCommandHandlerImpl(std::shared_ptr<domain::UserChannelRepository>,
                                       std::shared_ptr<domain::UserRepository>,
                                       std::shared_ptr<domain::ChannelRepository>);

    void execute(const AddUserToChannelCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
