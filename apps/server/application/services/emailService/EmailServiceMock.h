#pragma once

#include <gmock/gmock.h>

#include "EmailService.h"

namespace server::application
{
class EmailServiceMock : public EmailService
{
public:
    MOCK_METHOD(void, sendEmail, (const SendEmailPayload& payload), (const override));
};
}
