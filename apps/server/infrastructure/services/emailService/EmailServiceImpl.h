#pragma once

#include <memory>

#include "httpClient/HttpClient.h"
#include "server/application/services/emailService/EmailService.h"

namespace server::application
{
class EmailServiceImpl : public EmailService
{
public:
    EmailServiceImpl(std::shared_ptr<common::httpClient::HttpClient>, std::string sendApiKey);

    void sendEmail(const SendEmailPayload& payload) const override;

private:
    std::shared_ptr<common::httpClient::HttpClient> httpClient;
    std::string sendGridApiKey;
};
}
