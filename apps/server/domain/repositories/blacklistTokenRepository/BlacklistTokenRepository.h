#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../../entities/blacklistToken/BlacklistToken.h"
#include "payloads/CreateBlacklistTokenPayload.h"
#include "server/domain/repositories/blacklistTokenRepository/payloads/FindBlacklistTokenByTokenPayload.h"

namespace server::domain
{
class BlacklistTokenRepository
{
public:
    virtual ~BlacklistTokenRepository() = default;

    virtual std::shared_ptr<BlacklistToken> createBlacklistToken(const CreateBlacklistTokenPayload&) const = 0;
    virtual std::optional<std::shared_ptr<BlacklistToken>>
    findBlacklistTokenByToken(const FindBlacklistTokenByTokenPayload&) const = 0;
};
}
