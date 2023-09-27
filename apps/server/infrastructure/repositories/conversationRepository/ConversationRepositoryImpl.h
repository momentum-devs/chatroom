#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "server/domain/repositories/conversationRepository/ConversationRepository.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ConversationRepositoryImpl : public domain::ConversationRepository
{
public:
    ConversationRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::shared_ptr<ConversationMapper>,
                               std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>);

    domain::Conversation createConversation(const domain::CreateConversationPayload&) const;
    std::optional<domain::Conversation> findConversationById(const domain::FindConversationByIdPayload&) const;
    std::optional<domain::Conversation>
    findConversationByUserAndRecipientIds(const domain::FindConversationByUserAndRecipientIdsPayload&) const;
    std::optional<domain::Conversation>
    findConversationByChannelId(const domain::FindConversationByChannelIdPayload&) const;
    void deleteConversation(const domain::DeleteConversationPayload&) const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<ConversationMapper> conversationMapper;
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
};
}
