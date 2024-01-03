#include "BlacklistTokenRepositoryImpl.h"

#include "fmt/format.h"

#include "BlacklistToken.odb.h"
#include "server/infrastructure/errors/BlacklistTokenRepositoryError.h"

namespace server::infrastructure
{
BlacklistTokenRepositoryImpl::BlacklistTokenRepositoryImpl(
    std::shared_ptr<odb::sqlite::database> dbInit, std::shared_ptr<BlacklistTokenMapper> blacklistTokenMapperInit)
    : db{std::move(dbInit)}, blacklistTokenMapper{std::move(blacklistTokenMapperInit)}
{
}

std::shared_ptr<domain::BlacklistToken>
BlacklistTokenRepositoryImpl::createBlacklistToken(const domain::CreateBlacklistTokenPayload& payload) const
{
    try
    {
        {
            const auto blacklistToken = std::make_shared<BlacklistToken>(payload.id, payload.token, payload.expiresAt);

            odb::transaction transaction(db->begin());

            db->persist(blacklistToken);

            transaction.commit();

            return blacklistTokenMapper->mapToDomainBlacklistToken(blacklistToken);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::BlacklistTokenRepositoryError{error.what()};
    }
}

std::optional<std::shared_ptr<domain::BlacklistToken>>
BlacklistTokenRepositoryImpl::findBlacklistTokenByToken(const domain::FindBlacklistTokenByTokenPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<BlacklistToken> query;

        std::shared_ptr<BlacklistToken> blacklistToken(db->query_one<BlacklistToken>(query::token == payload.token));

        transaction.commit();

        if (!blacklistToken)
        {
            return std::nullopt;
        }

        return blacklistTokenMapper->mapToDomainBlacklistToken(blacklistToken);
    }
    catch (const std::exception& error)
    {
        throw errors::BlacklistTokenRepositoryError{error.what()};
    }
}

}
