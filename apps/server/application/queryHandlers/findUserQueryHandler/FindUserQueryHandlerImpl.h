#pragma once

#include <memory>

#include "FindUserQueryHandler.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class FindUserQueryHandlerImpl : public FindUserQueryHandler
{
public:
    explicit FindUserQueryHandlerImpl(std::shared_ptr<domain::UserRepository>);

    FindUserQueryHandlerResult execute(const FindUserQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
