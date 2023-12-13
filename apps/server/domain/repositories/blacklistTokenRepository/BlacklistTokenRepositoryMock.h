#pragma once

#include <gmock/gmock.h>

#include "BlacklistTokenRepository.h"

namespace server::domain
{
class BlacklistTokenRepositoryMock : public BlacklistTokenRepository
{
public:
    MOCK_METHOD(std::shared_ptr<BlacklistToken>, createBlacklistToken, (const CreateBlacklistTokenPayload&),
                (const override));
    MOCK_METHOD(std::optional<std::shared_ptr<BlacklistToken>>, findBlacklistTokenByToken,
                (const FindBlacklistTokenByTokenPayload&), (const override));
};
}
