#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>
#include <vector>

#include "server/domain/repositories/reactionRepository/ReactionRepository.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ReactionRepositoryImpl : public domain::ReactionRepository
{
public:
    ReactionRepositoryImpl(std::shared_ptr<odb::sqlite::database>, std::shared_ptr<ReactionMapper>,
                           std::shared_ptr<UserMapper>, std::shared_ptr<MessageMapper>);

    domain::Reaction createReaction(const domain::CreateReactionPayload&) const;
    std::optional<domain::Reaction> findReaction(const domain::FindReactionPayload&) const;
    std::optional<domain::Reaction> findReactionById(const domain::FindReactionByIdPayload&) const;
    std::vector<domain::Reaction> findReactionsByMessageId(const domain::FindReactionsByMessageIdPayload&) const;
    void deleteReaction(const domain::DeleteReactionPayload&) const;

private:
    std::shared_ptr<odb::sqlite::database> db;
    std::shared_ptr<ReactionMapper> reactionMapper;
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<MessageMapper> messageMapper;
};
}
