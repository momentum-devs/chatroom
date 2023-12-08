#pragma once

#include <memory>

#include "FindMessageReactionsQueryHandler.h"
#include "server/domain/repositories/reactionRepository/ReactionRepository.h"

namespace server::application
{
class FindMessageReactionsQueryHandlerImpl : public FindMessageReactionsQueryHandler
{
public:
    explicit FindMessageReactionsQueryHandlerImpl(std::shared_ptr<domain::ReactionRepository>);

    FindMessageReactionsQueryHandlerResult execute(const FindMessageReactionsQueryHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ReactionRepository> reactionRepository;
};
}
