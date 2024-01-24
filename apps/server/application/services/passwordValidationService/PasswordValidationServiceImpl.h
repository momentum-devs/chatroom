#pragma once

#include "PasswordValidationService.h"

namespace server::application
{
class PasswordValidationServiceImpl : public PasswordValidationService
{
public:
    void validate(const std::string& password) const override;
};
}
