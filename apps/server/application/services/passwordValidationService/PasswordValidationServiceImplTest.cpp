#include "PasswordValidationServiceImpl.h"

#include "gtest/gtest.h"

#include "server/application/errors/OperationNotValidError.h"

using namespace ::testing;
using namespace server;
using namespace server::application;

class PasswordValidationServiceImplTest : public Test
{
public:
    PasswordValidationServiceImpl passwordValidationService;
};

TEST_F(PasswordValidationServiceImplTest, shouldThrowExceptionWhenPasswordIsTooShort)
{
    ASSERT_THROW(passwordValidationService.validate("Ab1234"), errors::OperationNotValidError);
}

TEST_F(PasswordValidationServiceImplTest, shouldThrowExceptionWhenPasswordDoesNotContainUppercaseLetter)
{
    ASSERT_THROW(passwordValidationService.validate("abcdefg1"), errors::OperationNotValidError);
}

TEST_F(PasswordValidationServiceImplTest, shouldThrowExceptionWhenPasswordDoesNotContainLowercaseLetter)
{
    ASSERT_THROW(passwordValidationService.validate("ABCDEFG1"), errors::OperationNotValidError);
}

TEST_F(PasswordValidationServiceImplTest, shouldThrowExceptionWhenPasswordDoesNotContainDigit)
{
    ASSERT_THROW(passwordValidationService.validate("Abcdefgh"), errors::OperationNotValidError);
}

TEST_F(PasswordValidationServiceImplTest, shouldThrowExceptionWhenPasswordIsTooLong)
{
    ASSERT_THROW(
        passwordValidationService.validate("AbcdefghijklmnopqrstuvwxyzAbcdefghijklmnopqrstuvwxyzAbcdefghijklmnopqrstuvw"
                                           "xyzAbcdefghijklmnopqrstuvwxyzAbcdefghijklmnopqrstuvwxyz1"),
        errors::OperationNotValidError);
}

TEST_F(PasswordValidationServiceImplTest, shouldNotThrowExceptionWhenPasswordIsValid)
{
    ASSERT_NO_THROW(passwordValidationService.validate("Abcdefgh1"));
}
