#pragma once

#include <memory>

#include "CreateUserChannelCommandHandler.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"

namespace server::application
{
class CreateUserChannelCommandHandlerImpl : public CreateUserChannelCommandHandler
{
public:
    explicit CreateUserChannelCommandHandlerImpl(std::shared_ptr<domain::UserChannelRepository>);

    CreateUserChannelCommandHandlerResult execute(const CreateUserChannelCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
