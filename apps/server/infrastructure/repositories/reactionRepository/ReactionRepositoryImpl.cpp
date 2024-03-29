#include "ReactionRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "fmt/format.h"
#include "Reaction.odb.h"
#include "server/infrastructure/errors/ReactionRepositoryError.h"
#include "server/infrastructure/errors/ResourceNotFoundError.h"

namespace server::infrastructure
{
ReactionRepositoryImpl::ReactionRepositoryImpl(std::shared_ptr<odb::sqlite::database> dbInit,
                                               std::shared_ptr<ReactionMapper> reactionMapperInit,
                                               std::shared_ptr<UserMapper> userMapperInit,
                                               std::shared_ptr<MessageMapper> messageMapperInit)
    : db{std::move(dbInit)},
      reactionMapper{std::move(reactionMapperInit)},
      userMapper{std::move(userMapperInit)},
      messageMapper{std::move(messageMapperInit)}
{
}

domain::Reaction ReactionRepositoryImpl::createReaction(const domain::CreateReactionPayload& payload) const
{
    try
    {
        {
            const auto user = userMapper->mapToPersistenceUser(payload.user);

            const auto message = messageMapper->mapToPersistenceMessage(payload.message);

            Reaction reaction{payload.id, payload.name, user, message};

            odb::transaction transaction(db->begin());

            db->persist(reaction);

            transaction.commit();

            return reactionMapper->mapToDomainReaction(reaction);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::ReactionRepositoryError{error.what()};
    }
}

std::vector<domain::Reaction> ReactionRepositoryImpl::findReactions(const domain::FindReactionsPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Reaction> Query;

        typedef odb::result<Reaction> Result;

        Result result =
            db->query<Reaction>(Query::user->id == payload.userId && Query::message->id == payload.messageId);

        std::vector<domain::Reaction> domainReactions;

        for (const auto& reaction : result)
        {
            domainReactions.push_back(reactionMapper->mapToDomainReaction(reaction));
        }

        transaction.commit();

        return domainReactions;
    }
    catch (const std::exception& error)
    {
        throw errors::ReactionRepositoryError{error.what()};
    }
}

std::optional<domain::Reaction>
ReactionRepositoryImpl::findReactionById(const domain::FindReactionByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Reaction> Query;

        std::shared_ptr<Reaction> reaction(db->query_one<Reaction>(Query::id == payload.id));

        transaction.commit();

        if (!reaction)
        {
            return std::nullopt;
        }

        return reactionMapper->mapToDomainReaction(*reaction);
    }
    catch (const std::exception& error)
    {
        throw errors::ReactionRepositoryError{error.what()};
    }
}

std::vector<domain::Reaction>
ReactionRepositoryImpl::findReactionsByMessageId(const domain::FindReactionsByMessageIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Reaction> Query;

        typedef odb::result<Reaction> Result;

        Result result = db->query<Reaction>(Query::message->id == payload.messageId);

        std::vector<domain::Reaction> domainReactions;

        for (const auto& reaction : result)
        {
            domainReactions.push_back(reactionMapper->mapToDomainReaction(reaction));
        }

        transaction.commit();

        return domainReactions;
    }
    catch (const std::exception& error)
    {
        throw errors::ReactionRepositoryError{error.what()};
    }
}

domain::Reaction ReactionRepositoryImpl::updateReaction(const domain::UpdateReactionPayload& payload) const
{
    try
    {
        {
            odb::transaction transaction(db->begin());

            typedef odb::query<Reaction> query;

            std::shared_ptr<Reaction> reaction(db->query_one<Reaction>(query::id == payload.reaction.getId()));

            if (!reaction)
            {
                throw errors::ResourceNotFoundError{
                    fmt::format("Reaction with id \"{}\" not found.", payload.reaction.getId())};
            }

            reaction->setName(payload.reaction.getName());

            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            db->update(*reaction);

            transaction.commit();
        }

        return *findReactionById({payload.reaction.getId()});
    }
    catch (const std::exception& error)
    {
        throw errors::ReactionRepositoryError{error.what()};
    }
}

void ReactionRepositoryImpl::deleteReaction(const domain::DeleteReactionPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<Reaction>(payload.reaction.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::ReactionRepositoryError{error.what()};
    }
}

}
