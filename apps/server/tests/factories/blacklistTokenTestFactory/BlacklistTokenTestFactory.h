#pragma once

#include <memory>

#include "BlacklistToken.h"
#include "server/domain/entities/blacklistToken/BlacklistToken.h"

namespace server::tests
{
class BlacklistTokenTestFactory
{
public:
    std::shared_ptr<domain::BlacklistToken> createDomainBlacklistToken();
    std::shared_ptr<infrastructure::BlacklistToken> createPersistentBlacklistToken();
};
}
