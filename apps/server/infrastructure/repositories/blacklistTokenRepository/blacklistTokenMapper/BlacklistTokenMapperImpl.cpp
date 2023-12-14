#include "BlacklistTokenMapperImpl.h"

namespace server::infrastructure
{
std::shared_ptr<domain::BlacklistToken>
BlacklistTokenMapperImpl::mapToDomainBlacklistToken(std::shared_ptr<BlacklistToken> blacklistToken) const
{
    const auto id = blacklistToken->getId();
    const auto token = blacklistToken->getToken();
    const auto expiresAt = blacklistToken->getExpiresAt();

    return std::make_shared<domain::BlacklistToken>(id, token, expiresAt);
}

}
