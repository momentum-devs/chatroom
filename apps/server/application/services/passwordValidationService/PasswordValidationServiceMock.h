#pragma once

#include <gmock/gmock.h>

#include "PasswordValidationService.h"

namespace server::application
{
class PasswordValidationServiceMock : public PasswordValidationService
{
public:
    MOCK_METHOD(void, validate, (const std::string& password), (const override));
};
}
