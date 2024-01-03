#include "MessageRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "fmt/format.h"
#include "Message.odb.h"
#include "server/infrastructure/errors/MessageRepositoryError.h"
#include "server/infrastructure/errors/ResourceNotFoundError.h"

namespace server::infrastructure
{
MessageRepositoryImpl::MessageRepositoryImpl(std::shared_ptr<odb::sqlite::database> dbInit,
                                             std::shared_ptr<MessageMapper> messageMapperInit,
                                             std::shared_ptr<UserMapper> userMapperInit,
                                             std::shared_ptr<ChannelMapper> channelMapperInit,
                                             std::shared_ptr<GroupMapper> groupMapperInit)
    : db{std::move(dbInit)},
      messageMapper{std::move(messageMapperInit)},
      userMapper{std::move(userMapperInit)},
      channelMapper{std::move(channelMapperInit)},
      groupMapper{std::move(groupMapperInit)}
{
}

std::shared_ptr<domain::Message> MessageRepositoryImpl::createMessage(const domain::CreateMessagePayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto sender = userMapper->mapToPersistenceUser(payload.sender);

            const auto channel = payload.channel ? channelMapper->mapToPersistenceChannel(*payload.channel) : nullptr;

            const auto group = payload.group ? groupMapper->mapToPersistenceGroup(*payload.group) : nullptr;

            const auto message = std::make_shared<Message>(payload.id, payload.content, sender, channel, group,
                                                           currentDate, currentDate);

            odb::transaction transaction(db->begin());

            db->persist(message);

            transaction.commit();

            return messageMapper->mapToDomainMessage(message);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

std::optional<std::shared_ptr<domain::Message>>
MessageRepositoryImpl::findMessageById(const domain::FindMessageByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Message> Query;

        std::shared_ptr<Message> message(db->query_one<Message>(Query::id == payload.id));

        transaction.commit();

        if (!message)
        {
            return std::nullopt;
        }

        return messageMapper->mapToDomainMessage(message);
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

std::vector<std::shared_ptr<domain::Message>>
MessageRepositoryImpl::findMessagesByChannelId(const domain::FindMessagesByChannelIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Message> Query;

        auto result = db->query<Message>(
            Query(fmt::format(R"(WHERE "channel"='{}' ORDER BY "created_at" DESC LIMIT {} OFFSET {})",
                              payload.channelId, payload.limit, payload.offset)));

        std::vector<std::shared_ptr<domain::Message>> domainMessages;

        for (auto& message : result)
        {
            domainMessages.push_back(messageMapper->mapToDomainMessage(std::make_shared<Message>(message)));
        }

        transaction.commit();

        return domainMessages;
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

std::vector<std::shared_ptr<domain::Message>>
MessageRepositoryImpl::findMessagesByGroupId(const domain::FindMessagesByGroupIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Message> Query;

        auto result =
            db->query<Message>(Query(fmt::format(R"(WHERE "group"='{}' ORDER BY "created_at" DESC LIMIT {} OFFSET {})",
                                                 payload.groupId, payload.limit, payload.offset)));

        std::vector<std::shared_ptr<domain::Message>> domainMessages;

        for (auto& message : result)
        {
            domainMessages.push_back(messageMapper->mapToDomainMessage(std::make_shared<Message>(message)));
        }

        transaction.commit();

        return domainMessages;
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

std::shared_ptr<domain::Message> MessageRepositoryImpl::updateMessage(const domain::UpdateMessagePayload& payload) const
{
    try
    {
        {
            odb::transaction transaction(db->begin());

            typedef odb::query<Message> query;

            std::shared_ptr<Message> message(db->query_one<Message>(query::id == payload.message.getId()));

            if (!message)
            {
                throw errors::ResourceNotFoundError{
                    fmt::format("Message with id \"{}\" not found.", payload.message.getId())};
            }

            message->setContent(payload.message.getContent());

            db->update(*message);

            transaction.commit();
        }

        return *findMessageById({payload.message.getId()});
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

void MessageRepositoryImpl::deleteMessage(const domain::DeleteMessagePayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<Message>(payload.message.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

unsigned MessageRepositoryImpl::countMessagesByChannelId(const domain::CountMessagesByChannelIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Message> Query;

        auto result = db->query<Message>(Query::channel->id == payload.channelId);

        auto iter = result.begin();
        int count = 0;

        while (iter != result.end())
        {
            iter++;
            count++;
        };

        transaction.commit();

        return count;
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

unsigned MessageRepositoryImpl::countMessagesByGroupId(const domain::CountMessagesByGroupIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Message> Query;

        auto result = db->query<Message>(Query::group->id == payload.groupId);

        auto iter = result.begin();
        int count = 0;

        while (iter != result.end())
        {
            iter++;
            count++;
        };

        transaction.commit();

        return count;
    }
    catch (const std::exception& error)
    {
        throw errors::MessageRepositoryError{error.what()};
    }
}

}
