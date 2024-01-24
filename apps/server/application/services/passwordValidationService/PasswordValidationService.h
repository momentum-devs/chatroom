#pragma once

#include <string>

namespace server::application
{
class PasswordValidationService
{
public:
    virtual ~PasswordValidationService() = default;

    virtual void validate(const std::string& password) const = 0;
};
}
