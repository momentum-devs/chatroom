#include "ConversationRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Conversation.odb.h"
#include "server/infrastructure/errors/ConversationRepositoryError.h"

namespace server::infrastructure
{
ConversationRepositoryImpl::ConversationRepositoryImpl(std::shared_ptr<odb::pgsql::database> dbInit,
                                                       std::shared_ptr<ConversationMapper> conversationMapperInit,
                                                       std::shared_ptr<UserMapper> userMapperInit,
                                                       std::shared_ptr<ChannelMapper> channelMapperInit)
    : db{std::move(dbInit)},
      conversationMapper{std::move(conversationMapperInit)},
      userMapper{std::move(userMapperInit)},
      channelMapper{std::move(channelMapperInit)}
{
}

domain::Conversation
ConversationRepositoryImpl::createConversation(const domain::CreateConversationPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto user = payload.user ? userMapper->mapToPersistenceUser(*payload.user) : nullptr;

            const auto recipient = payload.recipient ? userMapper->mapToPersistenceUser(*payload.recipient) : nullptr;

            const auto channel = payload.channel ? channelMapper->mapToPersistenceChannel(*payload.channel) : nullptr;

            Conversation conversation{payload.id, user, recipient, channel, currentDate, currentDate};

            odb::transaction transaction(db->begin());

            db->persist(conversation);

            transaction.commit();

            return conversationMapper->mapToDomainConversation(conversation);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::ConversationRepositoryError{error.what()};
    }
}

std::optional<domain::Conversation>
ConversationRepositoryImpl::findConversationById(const domain::FindConversationByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Conversation> Query;

        std::shared_ptr<Conversation> conversation(db->query_one<Conversation>(Query::id == payload.id));

        transaction.commit();

        if (!conversation)
        {
            return std::nullopt;
        }

        return conversationMapper->mapToDomainConversation(*conversation);
    }
    catch (const std::exception& error)
    {
        throw errors::ConversationRepositoryError{error.what()};
    }
}

std::optional<domain::Conversation> ConversationRepositoryImpl::findConversationByUserAndRecipientIds(
    const domain::FindConversationByUserAndRecipientIdsPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Conversation> Query;

        std::shared_ptr<Conversation> conversation(db->query_one<Conversation>(
            Query::user->id == payload.userId && Query::recipient->id == payload.recipientId));

        if (!conversation)
        {
            conversation = db->query_one<Conversation>(Query::user->id == payload.recipientId &&
                                                       Query::recipient->id == payload.userId);
        }

        transaction.commit();

        if (!conversation)
        {
            return std::nullopt;
        }

        return conversationMapper->mapToDomainConversation(*conversation);
    }
    catch (const std::exception& error)
    {
        throw errors::ConversationRepositoryError{error.what()};
    }
}

std::optional<domain::Conversation>
ConversationRepositoryImpl::findConversationByChannelId(const domain::FindConversationByChannelIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Conversation> Query;

        std::shared_ptr<Conversation> conversation(
            db->query_one<Conversation>(Query::channel->id == payload.channelId));

        transaction.commit();

        if (!conversation)
        {
            return std::nullopt;
        }

        return conversationMapper->mapToDomainConversation(*conversation);
    }
    catch (const std::exception& error)
    {
        throw errors::ConversationRepositoryError{error.what()};
    }
}

void ConversationRepositoryImpl::deleteConversation(const domain::DeleteConversationPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<Conversation>(payload.conversation.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::ConversationRepositoryError{error.what()};
    }
}

}
