#include "PasswordValidationServiceImpl.h"

#include "server/application/errors/OperationNotValidError.h"

namespace server::application
{
void PasswordValidationServiceImpl::validate(const std::string& password) const
{
    if (password.length() < 8)
    {
        throw errors::OperationNotValidError("Password must be at least 8 characters long");
    }

    if (password.length() > 64)
    {
        throw errors::OperationNotValidError("Password must be at most 64 characters long");
    }

    if (password.find_first_of("0123456789") == std::string::npos)
    {
        throw errors::OperationNotValidError("Password must contain at least one digit");
    }

    if (password.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos)
    {
        throw errors::OperationNotValidError("Password must contain at least one uppercase letter");
    }

    if (password.find_first_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos)
    {
        throw errors::OperationNotValidError("Password must contain at least one lowercase letter");
    }
}

}
