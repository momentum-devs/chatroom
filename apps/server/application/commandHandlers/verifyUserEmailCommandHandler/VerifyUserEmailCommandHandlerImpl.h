#pragma once

#include <memory>

#include "server/domain/repositories/userRepository/UserRepository.h"
#include "VerifyUserEmailCommandHandler.h"

namespace server::application
{
class VerifyUserEmailCommandHandlerImpl : public VerifyUserEmailCommandHandler
{
public:
    explicit VerifyUserEmailCommandHandlerImpl(std::shared_ptr<domain::UserRepository>);

    VerifyUserEmailCommandHandlerResult execute(const VerifyUserEmailCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
