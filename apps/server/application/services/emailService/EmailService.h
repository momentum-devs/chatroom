#pragma once

#include "payloads/SendEmailPayload.h"

namespace server::application
{
class EmailService
{
public:
    virtual ~EmailService() = default;

    virtual void sendEmail(const SendEmailPayload& payload) const = 0;
};
}
