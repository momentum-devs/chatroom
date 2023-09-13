#pragma once

#include <memory>

#include "LogoutUserCommandHandler.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class LogoutUserCommandHandlerImpl : public LogoutUserCommandHandler
{
public:
    explicit LogoutUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>);

    void execute(const LogoutUserCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
