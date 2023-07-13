#pragma once

#include "server/application/services/emailService/EmailService.h"

namespace server::application
{
class EmailServiceImpl : public EmailService
{
public:
    void sendEmail(const SendEmailPayload& payload) const override;
};
}
