#pragma once

#include <gmock/gmock.h>

#include "TokenService.h"

namespace server::application
{
class TokenServiceMock : public TokenService
{
public:
    MOCK_METHOD(std::string, createToken, (const std::string& userId), (const override));
    MOCK_METHOD(VerifyTokenResult, verifyToken, (const std::string& token), (const override));
    MOCK_METHOD(void, invalidateToken, (const std::string& token), (const override));
};
}
