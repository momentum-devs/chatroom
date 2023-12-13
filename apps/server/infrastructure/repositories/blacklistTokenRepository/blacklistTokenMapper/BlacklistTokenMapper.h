#pragma once

#include "../../../../domain/entities/blacklistToken/BlacklistToken.h"
#include "../../../database/entities/BlacklistToken.h"

namespace server::infrastructure
{
class BlacklistTokenMapper
{
public:
    virtual ~BlacklistTokenMapper() = default;

    virtual std::shared_ptr<domain::BlacklistToken>
        mapToDomainBlacklistToken(std::shared_ptr<BlacklistToken>) const = 0;
};
}
