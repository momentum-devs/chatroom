#pragma once

#include <memory>

#include "DeleteUserCommandHandler.h"
#include "server/domain/repositories/UserRepository.h"

namespace server::application
{
class DeleteUserCommandHandlerImpl : public DeleteUserCommandHandler
{
public:
    explicit DeleteUserCommandHandlerImpl(std::shared_ptr<domain::UserRepository>);

    void execute(const DeleteUserCommandHandlerPayload&) override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
