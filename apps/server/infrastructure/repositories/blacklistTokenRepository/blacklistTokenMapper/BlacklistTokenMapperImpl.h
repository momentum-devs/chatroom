#pragma once

#include "BlacklistTokenMapper.h"

namespace server::infrastructure
{
class BlacklistTokenMapperImpl : public BlacklistTokenMapper
{
public:
    std::shared_ptr<domain::BlacklistToken> mapToDomainBlacklistToken(std::shared_ptr<BlacklistToken>) const override;
};
}
