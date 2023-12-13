#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>
#include <vector>

#include "../../../domain/repositories/blacklistTokenRepository/BlacklistTokenRepository.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/blacklistTokenMapper/BlacklistTokenMapper.h"

namespace server::infrastructure
{
class BlacklistTokenRepositoryImpl : public domain::BlacklistTokenRepository
{
public:
    BlacklistTokenRepositoryImpl(std::shared_ptr<odb::sqlite::database>, std::shared_ptr<BlacklistTokenMapper>);

    std::shared_ptr<domain::BlacklistToken>
    createBlacklistToken(const domain::CreateBlacklistTokenPayload&) const override;
    std::optional<std::shared_ptr<domain::BlacklistToken>>
    findBlacklistTokenByToken(const domain::FindBlacklistTokenByTokenPayload&) const override;

private:
    std::shared_ptr<odb::sqlite::database> db;
    std::shared_ptr<BlacklistTokenMapper> blacklistTokenMapper;
};
}
