#pragma once

#include <memory>

#include "FindUserByEmailQueryHandler.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class FindUserByEmailQueryHandlerImpl : public FindUserByEmailQueryHandler
{
public:
    explicit FindUserByEmailQueryHandlerImpl(std::shared_ptr<domain::UserRepository>);

    FindUserByEmailQueryHandlerResult execute(const FindUserByEmailQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
