#pragma once

#include <gmock/gmock.h>

#include "HashService.h"

namespace server::application
{
class HashServiceMock : public HashService
{
public:
    MOCK_METHOD(std::string, hash, (const std::string& data), (const override));
    MOCK_METHOD(bool, compare, (const std::string& plainData, const std::string& hashedData), (const override));
};
}
